def pretty_format_obj(data):
    return pretty_format(data, '', 0)


def pretty_format(data, output, level, key='', is_last=False):
    new_line = '\n' + '    ' * level
    comma = '' if is_last or not level else ','

    if isinstance(data, list):
        output += new_line + str(key) + '['
        for item in data[:-1]:
            output = pretty_format(item, output, level + 1)
        output = pretty_format(data[-1], output, level + 1, is_last=True)
        output += new_line + ']' + comma

    elif isinstance(data, dict):
        output += new_line + str(key) + '{'
        for k in data.keys()[:-1]:
            output = pretty_format(data[k], output, level + 1, key=k + ': ')
        output = pretty_format(data[data.keys()[-1]], output, level + 1, key=data.keys()[-1] + ': ', is_last=True)
        output += new_line + '}' + comma

    else:
        output += new_line + str(key) + str(data) + comma

    return output


print pretty_format_obj([1, {'a': [1, 2], 'b': 'hi'}])
