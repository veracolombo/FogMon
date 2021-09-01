(defrule ctr_l
 (test (IsBatteryTooLow "fogmon_follower2_adp_1.fogmon_fm_net_adp"))
 =>
 (ChangeTimeReportLeader 50 "fogmon_follower2_adp_1.fogmon_fm_net_adp")
)