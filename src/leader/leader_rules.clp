(deftemplate metric_state
    (slot node)
    (slot metric)
    (slot state)
)

(defrule loadFacts (declare (salience 20))
 =>
 (LoadFacts)
)

(defrule battery_too_low (declare (salience 10))
 (metric_state (node ?x) (metric battery) (state too_low))
 (exists (metric_state (node ?x) (metric ?y) (state ?z))
         (test (neq ?y battery))
 )
 =>
 (SendDisableMetrics ?x (create$ free_cpu free_memory free_disk))
)