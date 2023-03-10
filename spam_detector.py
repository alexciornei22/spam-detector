#!/usr/bin/python3
"""Dummy docstring"""

import task1_utils as task1
import task2_utils as task2

SPAM_SCORE = 35

nr_files = task1.count_files()
nr_keywords, keyword_list = task1.get_keywords(nr_files)
task1.get_stdev(keyword_list, nr_keywords, nr_files)
task1.print_keywords(nr_keywords, keyword_list)

spammer_list = task2.get_spammers(task2.SPAMMERS_FILE)
scores = task2.scan_emails(nr_files, spammer_list, keyword_list)
task2.print_results(task2.PREDICTION_FILE, scores, SPAM_SCORE)
