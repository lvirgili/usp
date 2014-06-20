(TeX-add-style-hook "pres"
 (lambda ()
    (LaTeX-add-environments
     "mydef")
    (TeX-run-style-hooks
     "graphicx"
     "amsthm"
     "algorithm"
     "algorithmic"
     "minted"
     "fontenc"
     "T1"
     "inputenc"
     "utf8"
     "babel"
     "brazilian"
     "latex2e"
     "beamer10"
     "beamer")))

