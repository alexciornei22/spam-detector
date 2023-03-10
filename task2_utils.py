#!/usr/bin/python3
"""Dummy docstring"""

from io import TextIOWrapper
import task1_utils as task1
import os

SPAMMERS_FILE = open('./data/spammers', 'r')
PREDICTION_FILE = open('./prediction.out', 'w')

def get_spammers(file: TextIOWrapper):
    text = file.readlines()
    spammer_list = []

    for line in text[1:]:
        user, score = line.split()
        spammer = {
            'user': user,
            'score': int(score)
        }
        spammer_list.append(spammer)
    return spammer_list

def get_score(email, avg_size: float, keyword_list: 'list[task1.Keyword]'):
    score = 0
    i = email['index']

    keywords_score = sum([keyword.app[i] for keyword in keyword_list])
    keywords_score *= avg_size / email['size']
    caps_score = 1 if email['nr_caps'] > 0.5 * email['size'] else 0
    spammer_score = email['spammer']

    score = 10 * keywords_score + 30 * caps_score + spammer_score
    return score

def scan_emails(nr_files: int, spammer_list: 'list[dict]', keyword_list: 'list[task1.Keyword]'):
    emails_list = []
    avg_size = 0
    for i in range(0, nr_files):
        email_file = open(os.path.join(task1.EMAILS_DIR, str(i)))
        text = email_file.read()
        email_data = {
            'index': int(i),
            'size': 0,
            'score': 0,
            'nr_caps': 0,
            'spammer': 0
        }
        email_data['size'] = len(text)
        avg_size += email_data['size']
        email_data['nr_caps'] = sum(1 for char in text if char.isupper())
        
        for spammer in spammer_list:
            if text.find(spammer['user']) != -1:
                email_data['spammer'] = spammer['score']
        emails_list.append(email_data)

    avg_size /= nr_files
    scores = []
    for email in emails_list:
        scores.append(get_score(email, avg_size, keyword_list))
    return scores
    
def print_results(file: TextIOWrapper, scores: list, SPAM_SCORE: int):
    for score in scores:
        result = 0 if score < SPAM_SCORE else 1
        file.write(str(result) + '\n')