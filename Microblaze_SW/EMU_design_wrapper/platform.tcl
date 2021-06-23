# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct E:\Xilinx\Projects\ElectronicMonitoringUnit\Microblaze_SW\EMU_design_wrapper\platform.tcl
# 
# OR launch xsct and run below command.
# source E:\Xilinx\Projects\ElectronicMonitoringUnit\Microblaze_SW\EMU_design_wrapper\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {EMU_design_wrapper}\
-hw {E:\Xilinx\Projects\ElectronicMonitoringUnit\EMU\EMU_design_wrapper.xsa}\
-fsbl-target {psu_cortexa53_0} -out {E:/Xilinx/Projects/ElectronicMonitoringUnit/Microblaze_SW}

platform write
domain create -name {standalone_microblaze_0} -display-name {standalone_microblaze_0} -os {standalone} -proc {microblaze_0} -runtime {cpp} -arch {32-bit} -support-app {hello_world}
platform generate -domains 
platform active {EMU_design_wrapper}
platform generate -quick
platform generate
bsp reload
bsp reload
platform active {EMU_design_wrapper}
bsp reload
bsp reload
platform config -updatehw {E:/Xilinx/Projects/ElectronicMonitoringUnit/EMU/EMU_design_wrapper.xsa}
bsp reload
platform generate
bsp reload
bsp reload
bsp reload
platform config -updatehw {E:/Xilinx/Projects/ElectronicMonitoringUnit/EMU/EMU_design_wrapper.xsa}
platform generate -domains 
bsp reload
bsp reload
platform config -updatehw {E:/Xilinx/Projects/ElectronicMonitoringUnit/EMU/EMU_Top.xsa}
bsp reload
bsp reload
platform config -updatehw {E:/Xilinx/Projects/ElectronicMonitoringUnit/EMU/EMU_Top.xsa}
platform config -updatehw {E:/Xilinx/Projects/ElectronicMonitoringUnit/Microblaze_SW/EMU_design_wrapper.xsa}
platform config -updatehw {E:/Xilinx/Projects/ElectronicMonitoringUnit/EMU/EMU_design_wrapper.xsa}
platform generate -domains standalone_microblaze_0 
