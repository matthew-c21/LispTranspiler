(format (or #f #t 1))
(format (or #f #f 1 #f))
(format (and 1 2 3 #t))
(format (and 1 3 #f 7))
(format (not #t))
(format (not #f))
(format (not 3i))
(format (> 5 3i))
(format (< 1 2 3))
(format (>= 5 4 4 3.9 3.75 (+ 2 3i) (- 1) (- 7/2)))
