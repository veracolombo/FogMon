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

(defrule ch_param_btl (declare (salience 10))
 (my_metric_state (metric battery) (state too_low))
 (not (my_parameters (time_report 60) (time_tests 60) (time_latency 60)))
 =>
 (ChangeTimeReport 60)
 (ChangeTimeTests 60)
 (ChangeTimeLatency 60)
)