#!/bin/bash

echo '*** Generating html ***'
asciidoctor OpenGLQtTutorial.adoc
echo '*** Generating pdf ***'
asciidoctor-pdf -a pdf-theme=OpenGLQtTutorial-pdf-theme.yml -r ./rouge_theme.rb OpenGLQtTutorial.adoc

