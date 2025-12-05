;; Elementary differentials as rooted trees (A000081)
(define (elementary-differentials order)
  "Generate all elementary differentials up to given order"
  (match order
    [1 '(f)]  ; Single node
    [2 '((f' f))]  ; One edge
    [3 '((f'' f f) 
         (f' (f' f)))]  ; Two trees of order 3
    [4 '((f''' f f f)
         (f'' (f' f) f)
         (f' (f'' f f))
         (f' (f' (f' f))))]))  ; 4 trees of order 4
    [5 '((f'''' f f f f)
         (f''' (f' f) f f)
         (f'' (f'' f f) f)
         (f'' (f' (f' f)) f)
         (f' (f''' f f f))
         (f' (f'' (f' f) f))
         (f' (f'' f f))
         (f' (f'' f (f' f)))
         (f' (f' (f'' f f)))
         (f' (f' (f' f))))]))  ; 9 trees of order 5

