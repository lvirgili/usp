(TeX-add-style-hook "trabalho"
 (lambda ()
    (LaTeX-add-labels
     "fig:1"
     "eq:1")
    (TeX-add-symbols
     "linha")
    (TeX-run-style-hooks
     "algorithm"
     "algorithmic"
     "amssymb"
     "amsfonts"
     "graphicx"
     "anysize"
     "minted"
     "fontenc"
     "T1"
     "inputenc"
     "utf8"
     "babel"
     "brazilian"
     "latex2e"
     "art12"
     "article"
     "a4"
     "paper"
     "12pt")))

