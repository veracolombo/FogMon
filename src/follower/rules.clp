(defglobal ?*max-priority* = 100)
(defglobal ?*mid-priority* = 0)
(defglobal ?*min-priority* = -100)

(defglobal ?*min-time-report* = 10)
(defglobal ?*max-time-report* = 50)
(defglobal ?*n-metrics* = 3)
(defglobal ?*window* = 10)

(defrule ch_tr (declare (salience ?*max-priority*))
 =>
 (bind ?x (MetricInStateFor "free_cpu" "stable"))
 (bind ?y (MetricInStateFor "free_memory" "stable"))
 (bind ?z (MetricInStateFor "free_disk" "stable"))

 (ChangeTimeReport (GetTimeReport (+ ?x ?y ?z) 
                                  ?*n-metrics* 
                                  ?*window* 
                                  ?*min-time-report* 
                                  ?*max-time-report*)))

(defrule en_qos (declare (salience ?*mid-priority*))
 (test (>= (MetricInStateFor "free_cpu" "stable") 3))
 (test (>= (MetricInStateFor "free_memory" "stable") 3))
 (test (>= (MetricInStateFor "free_disk" "stable") 3))
 (test (>= (MetricInStateFor "free_cpu" "ok") 3))
 (test (>= (MetricInStateFor "free_memory" "ok") 3))
 (test (>= (MetricInStateFor "free_disk" "ok") 3))
 =>
 (EnableMetric "latency")
 (EnableMetric "bandwidth"))


(defrule dis_qos (declare (salience ?*mid-priority*))
 (or 
 (test (>= (MetricInStateFor "free_cpu" "unstable") 2))
 (test (>= (MetricInStateFor "free_memory" "unstable") 2))
 (test (>= (MetricInStateFor "free_disk" "unstable") 2))
 )
 =>
 (DisableMetric "latency")
 (DisableMetric "bandwidth"))
