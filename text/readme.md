# OpenGL + Qt Manual

Written in 'AsciiDoctor'

See: _https://asciidoctor.org_

## Install (Linux/Ubuntu)

```bash
> sudo apt install asciidoctor 

# Install ruby, and then:

> sudo gem install asciidoctor-pdf --pre
> sudo gem install rouge

# Install rouge syntax highlighter extension
> sudo gem install asciidoctor-rouge
> sudo gem install asciidoctor-mathematical
```

## Generating Documentation

```bash

# html export
> asciidoctor OpenGLQtTutorial.adoc

# pdf export
> asciidoctor-pdf OpenGLQtTutorial.adoc
```

PDF-Export using custom theme:

```bash
echo '*** Generating pdf ***'
asciidoctor-pdf -a pdf-theme=OpenGLQtTutorial-pdf-theme.yml OpenGLQtTutorial.adoc
```

PDF-Export using rouge syntax highlighter

```bash
echo '*** Generating pdf ***'
asciidoctor-pdf -a pdf-theme=OpenGLQtTutorial-pdf-theme.yml -r ./rouge_theme.rb OpenGLQtTutorial.adoc
```


## Writing notes

Use Editor: AsciidocFX _https://asciidocfx.com_

Mind:

- Pictures file names must only have one .



### Cross-referencing - best practice

When working with split documents (multiple adoc files pulled together with include),

First write the text, generate the html output from the master file, look at the html-code of the table of contents, for example

```html
<li><a href="#_master_algorithms">1.6. Master Algorithms</a>
<ul class="sectlevel3">
<li><a href="#_gauss_jacobi">1.6.1. Gauss-Jacobi</a></li>
<li><a href="#_gauss_seidel">1.6.2. Gauss-Seidel</a></li>
<li><a href="#_newton">1.6.3. Newton</a></li>
</ul>
</li>
<li><a href="#_error_control_and_time_step_adjustment">1.7. Error control and time step adjustment</a></li>
</ul>
</li>
<li><a href="#_graphical_user_interface">2. Graphical User Interface</a>
<ul class="sectlevel2">
<li><a href="#_welcome_page">2.1. Welcome page</a></li>
<li><a href="#_tool_bar">2.2. Tool bar</a></li>
<li><a href="#_slaves_view">2.3. Slaves view</a></li>
```

Now create cross references to a section via:

  <li><a href="#_tool_bar">2.2. Tool bar</a></li>

becomes

  <<_tool_bar, Tool bar>>

This works in html and pdf export as clickable link.


## TODO - Tool chain

PDFs need high-res images, HTML needs images
properly resized to 1000 pixel widths (depending on style sheet)

-> after html export, adjust png image file names to reference
   scaled versions -> need python script

## Screenshots

### MacOS

Cmd + Shift + 4 - dann Space, Screenshot wird auf dem Schreibtisch gespeichert

### Linux

Shutter

### Windows

SnagIt


