from os import listdir
from os.path import isfile, join
import string
import re

path_archivos_txt = "./txt"
archivos_txt = []
for f in listdir(path_archivos_txt):
    if isfile(join(path_archivos_txt, f)):
        archivos_txt.append(f)

archivo_grande = open("archivo_grande.txt", "w")

for i in range(4):
    for idx, nombre_txt in enumerate(archivos_txt):
        print(idx, nombre_txt)
        archivo_pequenho = open(join(path_archivos_txt, nombre_txt), "r")
        texto_archivo_pequenho = archivo_pequenho.read()
        texto_archivo_pequenho = texto_archivo_pequenho.translate(texto_archivo_pequenho.maketrans('', '', string.punctuation))
        #texto_archivo_pequenho = " ".join(texto_archivo_pequenho.split())
        texto_archivo_pequenho = re.sub('[ \t]+', ' ', texto_archivo_pequenho)
        texto_archivo_pequenho = texto_archivo_pequenho.lower()
        archivo_grande.write(texto_archivo_pequenho + " ")

max_sublist = len(archivos_txt) // 4
for idx, nombre_txt in enumerate(archivos_txt[0:max_sublist]):
    print(idx, nombre_txt)
    archivo_pequenho = open(join(path_archivos_txt, nombre_txt), "r")
    texto_archivo_pequenho = archivo_pequenho.read()
    texto_archivo_pequenho = texto_archivo_pequenho.translate(texto_archivo_pequenho.maketrans('', '', string.punctuation))
    #texto_archivo_pequenho = " ".join(texto_archivo_pequenho.split())
    texto_archivo_pequenho = re.sub('[ \t]+', ' ', texto_archivo_pequenho)
    texto_archivo_pequenho = texto_archivo_pequenho.lower()
    archivo_grande.write(texto_archivo_pequenho + " ")

archivo_grande.close() 

#print(len(archivos_txt))