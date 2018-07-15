import os
import sys

extensions = ['cpp']
source_dirs = ['src', 'Src', 'source', 'Source']

def GetExtension(_filename):
    return _filename.split(".")[-1]

def GetParent(_filename):
    return _filename.split(os.sep)[-1]

def GetProjectSourceFile(_directory):
    print(_directory)
    for dir_path, dir_names, file_names in os.walk(_directory):
        for f in file_names:
            file_extension = GetExtension(f)
            if file_extension in extensions and dir_path == _directory:
                return os.path.join(dir_path, f)
            parent_dir = GetParent(os.path.normpath(dir_path))
            if file_extension in extensions and parent_dir in source_dirs:
                return os.path.join(dir_path, f)
    return ""



print(GetProjectSourceFile(sys.argv[1]))
