(TeX-add-style-hook
 "report_final"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("article" "11pt")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("geometry" "a4" "margin=1in") ("inputenc" "utf8")))
   (TeX-run-style-hooks
    "latex2e"
    "article"
    "art11"
    "geometry"
    "inputenc"
    "graphicx"
    "tikz"
    "tabularx"
    "longtable")
   (TeX-add-symbols
    "checkmark"))
 :latex)

