from PIL import Image
import sys
import exifread
import os
from datetime import datetime


def main():
    args = sys.argv[1:]
    for file in args:
        with open(file, "rb") as image_file:
            tags = exifread.process_file(image_file)
            if tags:
                print("Données EXIF trouvées :")
            for tag, value in tags.items():
                print(f"{tag}: {value}")
            if not tags:
                print("Aucune donnée EXIF trouvée.")

        with Image.open(file) as img:
            s = os.stat(file)
            print("\nMétadonnées de l'image :")
            print(f"filename : { os.path.abspath(file).split('/')[-1]}")
            print(
                "Date de création :",
                datetime.fromtimestamp(s.st_ctime).strftime("%Y-%m-%d %H:%M"),
            )
            print(
                "Date de dernière modification :",
                datetime.fromtimestamp(s.st_mtime).strftime("%Y-%m-%d %H:%M"),
            )
            print(f"Format : {img.format}")
            print(f"Mode : {img.mode}")
            print(f"Taille : {img.size}")
            print(f"Palette : {img.palette}")
            print(f"Infos supplémentaires : {img.info}")
            print("-------------------------------------------------")


if __name__ == "__main__":
    main()
