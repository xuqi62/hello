#!/usr/bin/python2
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import numpy as np

total_mem = np.loadtxt("./data/output.csv")
max_total = np.max(total_mem)
max_total_string = "max total: " + str(max_total/1000) + "MB"

native_mem = np.loadtxt("./data/native_output.csv")
max_native = np.max(native_mem)
max_native_string = "max native: " + str(max_native/1000) + "MB"

graphics_mem = np.loadtxt("./data/graphics_output.csv")
max_graphics = np.max(graphics_mem)
max_graphics_string = "max graphics: " + str(max_graphics/1000) + "MB"

code_mem = np.loadtxt("./data/code_output.csv")
max_code = np.max(code_mem)
max_code_string = "max code: " + str(max_code/1000) + "MB"

total_mem_label = 'Total memory, ' + max_total_string
native_mem_label = 'Native memory, ' + max_native_string
graphics_mem_label = 'Graphics memory, ' + max_graphics_string
code_mem_label = 'Code memory, ' + max_code_string

plt.plot(total_mem, label=total_mem_label)
plt.plot(native_mem, label=native_mem_label)
plt.plot(graphics_mem, label=graphics_mem_label)
plt.plot(code_mem, label=code_mem_label)
plt.grid()
plt.legend()
plt.title('process memory usage')
plt.show()


