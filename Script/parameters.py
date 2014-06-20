from xml.dom import minidom
import codecs
import re
import os
import sys
reload(sys)
sys.setdefaultencoding('utf8')

def write_xml_file(path, xmlDom, option = {'encoding':'utf-8'}):
    """ Generate xml file with writer
    params:
        string path        xml file path
        Dom     xmlDom      xml dom
        dictionary option   writer option {'indent': '', 'addindent':' ', 'newl':'\n', 'encoding':'utf-8'}
    returns:
        bool                success return True else False
    """
    defaultOption = {'indent': '', 'addindent':' ', 'newl':'\n', 'encoding':'utf-8'}
    for k, v in defaultOption.iteritems():
        if k not in option:
            option[k] = v
    
    try:
        dirname = os.path.dirname(path)
        if not os.path.exists(dirname):
            os.makedirs(dirname)
        f=file(path, 'wb')
        writer = codecs.lookup(option['encoding'])[3](f)
        xmlDom.writexml(writer, encoding = option['encoding'], indent = option['indent'], \
                        addindent = option['addindent'], newl = option['newl'])
        writer.close()
        return True
    except:
        print('Write xml file failed.... file:{0}'.format(path))
        return False

def add_child(root, data):
    name = data[0]
    path = data[1]
    node_data = data[2]
    tags = path.split('/')
    current_node = root
    for tag in tags:
        node = None
        node_list = current_node.getElementsByTagName(tag)
        if len(node_list)==0:
            node = xmlDom.createElement(tag)
            current_node.appendChild(node)
        else:
            node = node_list[0]
        current_node = node

    node = xmlDom.createElement(name)
    node.setAttribute('type', '0')
    id = int(node_data['id'])
 
    for k, v in node_data.items():
        sub_node = xmlDom.createElement(k)
        text_node = xmlDom.createTextNode(v)
        sub_node.appendChild(text_node)
        node.appendChild(sub_node)

    current_node.appendChild(node)
    if not current_node.hasAttribute('lb'):
        current_node.setAttribute('lb', str(id))
        current_node.setAttribute('hb', str(id))
        return

    lb = int(current_node.getAttribute('lb'))
    hb = int(current_node.getAttribute('hb'))
    if id>hb:
        current_node.setAttribute('hb', str(id))
    if id<lb:
        current_node.setAttribute('lb', str(id))

def parse(line):
    pat1 = '^ParameterItem.*?\s+(\w+)\((\d+),\s*"\w+",\s*"([\w/]+)",\s*"(.*?)",\s*([-]?\d+),\s*([-]?\d+),\s*([-]?\d+),\s*"(.*?)"\)'
    pat2 = '^ParameterItem.*?\s+(\w+)\((\d+),\s*"\w+",\s*"([\w/]+)",\s*"(.*?)",\s*([-]?\d+),\s*([-]?\d+),\s*([-]?\d+)\)'
    pat3 = '^ParameterItem.*?\s+(\w+)\((\d+),\s*"\w+",\s*"([\w/]+)",\s*"(.*?)",\s*"(.*?)"\)'
    result = re.search(pat1, line)
    if result is None:
        result = re.search(pat2, line)
        if result is None:
            result = re.search(pat3, line)
            if result is None:
                return None

    groups = result.groups()
    name = groups[0]
    path = groups[2]
    node_data = {}
    node_data['id'] = groups[1]
    node_data['description'] = groups[3]
    if len(groups)==8:
        node_data['min'] = groups[4]
        node_data['max'] = groups[5]
        node_data['default'] = groups[6]
        node_data['unit'] = groups[7]
    elif len(groups)==7:
        node_data['min'] = groups[4]
        node_data['max'] = groups[5]
        node_data['default'] = groups[6]
        node_data['unit'] = ''
    else:
        node_data['min'] = 'null'
        node_data['max'] = 'null'
        node_data['default'] = groups[4]
        node_data['unit'] = ''

    return (name, path, node_data)


if __name__ == "__main__":
    # Create a xml dom
    xmlDom = minidom.Document()
    root = xmlDom.createElement('parameters')
    xmlDom.appendChild(root)

    filename = ''
    xmlfile = ''
    if len(sys.argv)>2:
        filename = sys.argv[1]
        xmlfile = sys.argv[2]
    for line in open(filename, 'r'):
        #print(line)
        data = parse(line)
        if data is None:
            continue
        #print(data)
        add_child(root, data)

    #print('\n' + xmlDom.toprettyxml(indent='  '))
    
    # Save xml file with encoding utf-8
    option = {'indent':'  ', 'addindent':'  ', 'newl':'\n', 'encoding':'utf8'}
    write_xml_file(xmlfile, xmlDom, option)
