(defglobal ?*max-priority* = 100)
(defglobal ?*min-priority* = -100)
(defglobal ?*min-time-report* = 10)
(defglobal ?*max-time-report* = 30)
(defglobal ?*n-metrics* = 3)
(defglobal ?*window* = 10)

(defrule ctr (declare (salience ?*max-priority*))
 =>
 (bind ?x (MetricInStateFor "free_cpu" "stable"))
 (bind ?y (MetricInStateFor "free_memory" "stable"))
 (bind ?z (MetricInStateFor "free_disk" "stable"))

 (ChangeTimeReport (GetTimeReport (+ ?x ?y ?z) 
                                  ?*n-metrics* 
                                  ?*window* 
                                  ?*min-time-report* 
                                  ?*max-time-report*)))

(defrule en_fcpu
 (test (< (MetricInStateFor "free_cpu" "unstable") 3))
 =>
 (DisableMetric "free_memory"))

(defrule dis_fcpu
 (test (>= (MetricInStateFor "free_cpu" "unstable") 3))
 =>
 (EnableMetric "free_memory"))

(defrule en_qos
 (test (> (MetricInStateFor "free_memory" "increasing") 1))
 =>
 (EnableMetric "latency")
 (EnableMetric "bandwidth"))