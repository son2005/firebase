#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
2016-12-02 jimmy.yin5@gmail.com

Convert google-services.json to xml
'''

import sys
import json
import traceback

def google_services_json_to_xml(json_file, xml_file='google-services.xml'):
    print('# read ' + json_file)
    f = open(json_file, 'r')
    lines = f.readlines()
    content = ''.join(lines)
    f.close()
    d = json.loads(content)

    template = '''<?xml version="1.0" encoding="utf-8"?>
<resources>
    <string name="default_web_client_id" translatable="false">{default_web_client_id}</string>
    <string name="gcm_defaultSenderId"   translatable="false">{gcm_defaultSenderId}</string>
    <string name="firebase_database_url" translatable="false">{firebase_database_url}</string>
    <string name="google_app_id"         translatable="false">{google_app_id}</string>
    <string name="google_api_key"        translatable="false">{google_api_key}</string>
    <string name="google_storage_bucket" translatable="false">{google_storage_bucket}</string>
</resources>
    '''

    ret = template.format(default_web_client_id=d['client'][0]['oauth_client'][0]['client_id']
                        , gcm_defaultSenderId=d['project_info']['project_number']
                        , firebase_database_url=d['project_info']['firebase_url']
                        , google_app_id=d['client'][0]['client_info']['mobilesdk_app_id']
                        , google_api_key=d['client'][0]['api_key'][0]['current_key']
                        , google_storage_bucket=d['project_info']['storage_bucket']
                        )

    print('# save [{0}]'.format(xml_file))
    f = open(xml_file, 'w')
    f.write(ret)
    f.close()

def main(argv):
    if len(argv) == 1:
        print('./json2xml.py [google-services.json')
        exit(1)
    elif len(argv) == 2:
        google_services_json_to_xml(argv[1])
    elif len(argv) == 3:
        google_services_json_to_xml(argv[1], xml_file=argv[2])
    else:
        print('./json2xml.py [google-services.json')

# ---------- main -------------
if __name__ == '__main__':
    try:
        sys.exit(main(sys.argv))
    except Exception as e:
        traceback.print_exc()
        sys.exit(1)
