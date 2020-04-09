#!/bin/bash

echo '*** Generating html ***'
asciidoctor OpenGLQtTutorial.adoc

echo '*** Generating pdf ***'
asciidoctor-pdf -a pdf-theme=pdf-theme.yml -r ./rouge_theme.rb -r asciidoctor-mathematical -a pdf-fontsdir="./fonts;GEM_FONTS_DIR" OpenGLQtTutorial.adoc

echo '*** Copying files to ../docs directory'

mv OpenGLQtTutorial.html ../docs/index.html
mv OpenGLQtTutorial.pdf ../docs

cp images/* ../docs/images

