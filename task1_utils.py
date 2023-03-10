#!/usr/bin/python3
"""Dummy docstring"""

import os
import numpy as np

EMAILS_DIR = './data/emails'
KEYWORDS_FILE = open('./data/keywords', 'r')

class Keyword():
    """Dummy docstring"""

    def __init__(self, word, nr_f):
        self.word = str(word)
        self.app = []
        for _ in range(0, nr_f):
            self.app.append(0)
        self.stdev = float(0)
        self.count = int(0)

# TASK 1:
def count_files():
    """Dummy docstring"""
    count = 0
    for file in os.listdir(EMAILS_DIR):
        if os.path.isfile(os.path.join(EMAILS_DIR, file)):
            count += 1
    return count

def get_keywords(nr_f):
    """Dummy docstring"""
    lines_list = KEYWORDS_FILE.read().splitlines()
    nr_kw = int(lines_list[0])
    init_list = []
    for i in range(0, nr_kw):
        init_list.append(Keyword(lines_list[i + 1], nr_f))
    return nr_kw, init_list

def get_stdev(kw_list: 'list[Keyword]', nr_kw: int, nr_f: int):
    """Dummy docstring"""
    for i in range(0, nr_f):
        email_file = open(os.path.join(EMAILS_DIR, str(i)), 'r')
        text = email_file.read()
        text = text[text.find('Body:'):]

        for keyword in kw_list:
            text_aux = text.casefold()
            pos_found = text_aux.find(keyword.word)
            while pos_found != -1:
                keyword.app[i] += 1
                text_aux = text_aux[pos_found + 1:]
                pos_found = text_aux.find(keyword.word)
        email_file.close()

    for i in range(0, nr_kw):
        med_arith = 0.0
        word_count = 0
        for email in range(0, nr_f):
            med_arith += kw_list[i].app[email]
            word_count += kw_list[i].app[email]
        med_arith /= nr_f

        stdev = 0.0
        for email in range(0, nr_f):
            email_val = kw_list[i].app[email] - med_arith
            email_val = email_val ** 2
            stdev += email_val
        stdev /= nr_f
        stdev = np.sqrt(stdev)
        kw_list[i].stdev = stdev
        kw_list[i].count = word_count

def print_keywords(nr_kw: int, kw_list: 'list[Keyword]'):
    """Dummy docstring"""
    stat_file = open('statistics.out', 'w')
    for i in range(0, nr_kw):
        stat_file.write(kw_list[i].word + ' ')
        stat_file.write(str(kw_list[i].count) + ' ')
        stat_file.write(format(kw_list[i].stdev, '.6f'))
        stat_file.write('\n')
    stat_file.close()
