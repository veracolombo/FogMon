(deftemplate my_metric_enabled
 (slot metric)
)

(deftemplate my_parameters
 (slot time_report)
 (slot time_tests)
 (slot time_latency)
)

(deftemplate my_metric_state
    (slot metric)
    (slot state)
)

(defrule loadFacts (declare (salience 20))
 =>
 (LoadFactsFollower)
)

(defrule ch_tr_stable (declare (salience 10))
 (my_metric_state (metric cpu) (state stable))
 =>
 (bind ?x (MetricInStateFor "cpu" "stable"))
 (ChangeTimeReport (GetTimeReport "stable" ?x))
)

(defrule ch_tr_unstable (declare (salience 10))
 (my_metric_state (metric cpu) (state unstable))
 =>
 (bind ?x (MetricInStateFor "cpu" "unstable"))
 (ChangeTimeReport (GetTimeReport "unstable" ?x))
)