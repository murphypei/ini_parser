# -*- coding: utf-8 -*-
# @Author: murphypei
# @Date:   2020-03-25 10:23:13
# @Last Modified by:   murphypei
# @Last Modified time: 2020-03-26 11:34:52

from configparser import ConfigParser
from easydict import EasyDict as edict


class IniParser():

    def __init__(self, ini_file):
        self._init_file = ini_file
        self._config = edict()
        self._default_section = "default"
        self._parser = ConfigParser(
            delimiters=('=', ','),
            comment_prefixes=('#'),
            inline_comment_prefixes=(';'),
            default_section=self._default_section,
            converters={
                'list': lambda x: [i.strip() for i in x.split(',')],
                'intlist': lambda x: [int(i.strip()) for i in x.split(',')],
                'floatlist': lambda x: [float(i.strip()) for i in x.split(',')]
            })

    def get_parser(self):
        self._parser.read(self._init_file, encoding='utf-8')
        return self._parser


if __name__ == '__main__':
    parser = IniParser("./config.ini").get_parser()
    print(parser.getintlist(section='model', option='output_dims'))
    print(parser.getint(section='tensorrt', option='device_id'))
    print(parser.getfloat(section='model', option='lstm_forget_bias'))
    print(parser.get(section='tensorrt', option='gie_file'))
