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

<<<<<<< HEAD
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
=======
(defrule disable_metrics_btl (declare (salience 10))
 (my_metric_state (metric battery) (state too_low))
 (my_metric_enabled (metric ?x&:(neq ?x battery)))
 =>
 (DisableMetrics (create$ ?x))
)

(defrule ch_param_btl (declare (salience 10))
 (my_metric_state (metric battery) (state too_low))
 (not (my_parameters (time_report 60) (time_tests 60) (time_latency 60)))
 =>
 (ChangeTimeReport 60)
 (ChangeTimeTests 60)
 (ChangeTimeLatency 60)
>>>>>>> parent of 5b5c071 (piccole modifiche)
)