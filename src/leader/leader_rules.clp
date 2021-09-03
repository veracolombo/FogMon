(defrule leader_adq_0 (declare (salience ?*min-priority*))
 (test (>= (MetricInStateFor "battery" "too_low") 6))
 =>
 (SendChangeServer)
)