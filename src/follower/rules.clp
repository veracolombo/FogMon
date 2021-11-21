(defglobal ?*max-priority* = 100)
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

(defrule en_mem_disk
 (test (>= (MetricInStateFor "free_cpu" "stable") 2))
 (test (not (IsMetricEnabled "free_memory")))
 (test (not (IsMetricEnabled "free_disk")))
 =>
 (EnableMetric "free_memory")
 (EnableMetric "free_disk"))

(defrule dis_mem_disk
 (test (>= (MetricInStateFor "free_cpu" "unstable") 3))
 (test (IsMetricEnabled "free_memory"))
 (test (IsMetricEnabled "free_disk"))
 =>
 (DisableMetric "free_memory")
 (DisableMetric "free_disk")
)


