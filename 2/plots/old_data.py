### -O2
O2_L2_miss_rate_5runs = 100 * np.array([      # in percents (100 *)
    [0.0314, 0.0301, 0.0356, 0.0330, 0.0313], 
    [0.0314, 0.0300, 0.0358, 0.0330, 0.0313], 
    [0.0314, 0.0300, 0.0358, 0.0331, 0.0314], 
    [0.0313, 0.0300, 0.0357, 0.0331, 0.0313], 
    [0.0315, 0.0300, 0.0356, 0.0331, 0.0314]
])

O2_L3_miss_rate_5runs = 100 * np.array([      # in percents (100 *)
    [3.750393e-07, 0.0026, 0.0064, 0.0082, 0.0070], 
    [3.752361e-07, 0.0026, 0.0065, 0.0080, 0.0070], 
    [3.752356e-07, 0.0034, 0.0063, 0.0080, 0.0070], 
    [3.752081e-07, 0.0018, 0.0064, 0.0080, 0.0070], 
    [3.752192e-07, 0.0016, 0.0063, 0.0081, 0.0070]
])

O2_MFLOPS_5runs = np.array([
    [491.5479, 324.6958, 244.7845, 227.8861, 228.7733], 
    [510.1678, 351.7653, 243.6273, 227.4609, 227.6405], 
    [499.5903, 401.6213, 244.2505, 227.7588, 228.7695], 
    [512.4094, 360.1106, 244.0123, 225.9582, 228.1091], 
    [509.6745, 399.0587, 245.3028, 226.9959, 228.6498]
])

### -O3 -fno-alias -xhost
O3_L2_miss_rate_5runs = 100 * np.array([      # in percents (100 *)
    [0.1239, 0.1304, 0.1600, 0.1608, 0.1614], 
    [0.1238, 0.1304, 0.1600, 0.1607, 0.1614], 
    [0.1238, 0.1304, 0.1600, 0.1608, 0.1614], 
    [0.1238, 0.1304, 0.1600, 0.1608, 0.1615], 
    [0.1238, 0.1304, 0.1600, 0.1608, 0.1614]
])

O3_L3_miss_rate_5runs = 100 * np.array([      # in percents (100 *)
    [1.507632e-06, 0.0059, 0.0012, 0.0018, 0.0017], 
    [3.028562e-06, 0.0056, 0.0012, 0.0018, 0.0017], 
    [1.514720e-06, 0.0064, 0.0012, 0.0018, 0.0017], 
    [3.786921e-06, 0.0050, 0.0012, 0.0018, 0.0017], 
    [4.544191e-06, 0.0049, 0.0012, 0.0018, 0.0017]
])

# Number is for DP, for AVX DP the number is equal +/- 5.
O3_MFLOPS_5runs = np.array([
    [1776.9151, 1627.7634, 1469.7033, 1330.5803, 1320.5122],
    [1778.2934, 1554.1183, 1495.4391, 1316.9242, 1322.0736],
    [1873.5473, 1668.4818, 1479.6065, 1305.5911, 1322.8326],
    [1890.2021, 1597.1130, 1502.2886, 1322.8273, 1321.8052], 
    [1755.8812, 1552.3627, 1464.1460, 1329.8544, 1326.3065]
])