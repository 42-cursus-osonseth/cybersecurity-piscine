from PIL import Image
import sys
import exifread
import os
from datetime import datetime
import argparse
import xml.etree.ElementTree as ET

EXTENSIONS = [".jpg", ".jpeg", ".png", ".gif", ".bmp", ".svg"]


def argsParser() -> dict:
    parser = argparse.ArgumentParser()
    parser.add_argument("files", nargs="*")
    parser.add_argument("-d","--DELETE",  action = 'store_true')
    args = parser.parse_args()
    args = vars(args)
    return args


def print_sys_info(file:str) -> None:
    s = os.stat(file)
    print("system info about image:")
    print(f"Fichier : {file}")
    print("Taille :", s.st_size, "octets")
    print("Créé le :", datetime.fromtimestamp(s.st_ctime))
    print("Modifié le :", datetime.fromtimestamp(s.st_mtime))



def strip_image(files: list) -> None:
    for file in files:
        ext = os.path.splitext(file)[1].lower()
        if ext != ".svg":
            with Image.open(file) as img:
                stripped_img = Image.new(img.mode, img.size)
                stripped_img.putdata(img.getdata())
            base = os.path.splitext(os.path.basename(file))[0]
            stripped_img.save(f"{base}_stripped{ext}")

def main():

    args = argsParser()
    print(args)

    if args["DELETE"]:
       strip_image(args["files"])
       exit(0)

    
    for file in args["files"]:
        ext = os.path.splitext(file)[1].lower()
        with open(file, "rb") as image_file:
            tags = exifread.process_file(image_file)
            print("metadonnes EXIF : ")
            if tags:
                for tag, value in tags.items():
                    print(f"{tag}: {value}")
            if not tags:
                print("No EXIF ​​data found")
        print_sys_info(file)
        if ext != ".svg":
            with Image.open(file) as img:
                print("\nImage properties:")
                print(f"Format : {img.format}")
                print(f"Mode : {img.mode}")
                print(f"Taille : {img.size}")
                print(f"Palette : {img.palette}")
                print(f"info : {img.info}")
                print("-------------------------------------------------")
        else:
             print("-------------------------------------------------")

if __name__ == "__main__":
    main()
