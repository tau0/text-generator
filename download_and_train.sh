#!/usr/bin/env bash

set -e

FILE_WITH_URLS=${1:-'urls.lst'}
ORDER=${2:-2}
ENCODING=${3:-'ru_RU.CP1251'}
FILE_TO_SAVE_MODEL=${4:-model.protobin}

TMP_FILE=$(mktemp /tmp/train_data.XXXXXX)

while read url; do
      curl -L "$url" >> $TMP_FILE
done <$FILE_WITH_URLS

cat $TMP_FILE | ./generator/text_generator fit ${ORDER} ${ENCODING} ${FILE_TO_SAVE_MODEL}

echo "Model saved to: $FILE_TO_SAVE_MODEL"

rm $TMP_FILE
