#!/bin/bash
# resize-to : redimensionne toutes les images du dossier courant
#             et les met dans un sous-dossier nommé d'après la taille
# Usage : resize-to 1920
#         resize-to 1200x800
#         resize-to 50%
#         resize-to 800x600^    (le ^ force le recadrage centré)

[ -z "$1" ] && echo "Usage: resize-to <taille>   (ex: resize-to 1920 ou resize-to 50%25)" && exit 1

SIZE="$1"
DIR="${SIZE//%/percent}"      # remplace % par "percent" pour le nom du dossier
DIR="${DIR//^/}"              # enlève le ^ éventuel du nom de dossier

mkdir -p "$DIR"

echo "Redimensionnement vers $SIZE → dossier ./$DIR"
for f in *.{jpg,JPG,jpeg,JPEG,png,PNG,webp,WEBP,gif,GIF}; do
    [ -f "$f" ] && convert "$f" -resize "$SIZE" "$DIR/${f%.*}.${f##*.}";
done

echo "Terminé ! $(ls "$DIR" | wc -l) images dans ./$DIR"
