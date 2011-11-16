#
ccflags-y = -DLINUX -D__linux__ -Idrivers/gpu/pvr \
	-DANDROID \
	-DPVR_BUILD_DIR="\"s3c_sgx_android\"" \
	-DSYS_CUSTOM_POWERLOCK_WRAP \
	-DSERVICES4 \
	-DSUPPORT_SRVINIT \
	-DSUPPORT_PERCONTEXT_PB \
	-DTRANSFER_QUEUE \
	-DSYS_USING_INTERRUPTS \
	-DSUPPORT_LINUX_X86_WRITECOMBINE \
	-DSUPPORT_LINUX_X86_PAT \
	-DPVR_SECURE_HANDLES \
	-DPVR_SECURE_FD_EXPORT \
	-DLDM_PLATFORM \
	-DSUPPORT_SGX_HWPERF \
	-DSUPPORT_MEMINFO_IDS \
	-DDISPLAY_CONTROLLER=s3c_lcd \
	-DSYS_SGX_ACTIVE_POWER_LATENCY_MS=100 \
	-DPVR_BUILD_DATE="" \
	-DSUPPORT_SGX \
	-DCLIENT_DRIVER_DEFAULT_WAIT_RETRIES=50 \
	-DPVR_LINUX_TIMERS_USING_WORKQUEUES \
	-DFLIP_TECHNIQUE_OVERLAY \
	-DPVR_LINUX_USING_WORKQUEUES \
	-DPVR_LINUX_MISR_USING_PRIVATE_WORKQUEUE \
	-DSUPPORT_SGX_NEW_STATUS_VALS \
	-DSGX_EARLYSUSPEND \
	-DSUPPORT_SGX_LOW_LATENCY_SCHEDULING \
	-DPVRSRV_MODNAME="\"pvrsrvkm"\" \
	-Idrivers/gpu/pvr/s5pc110 -Idrivers/gpu/pvr/sgx \
	-DSGX_CLK_CORE_DIV5 \
	-DSGX540 -DSUPPORT_SGX540 \
	-DSGX_CORE_REV=120 \
	-DSLSI_S5PC110

ccflags-$(CONFIG_SGX_540_BUILD_RELEASE) += \
	-DPVR_BUILD_TYPE="\"release\"" \
	-DRELEASE \
	-DSUPPORT_ACTIVE_POWER_MANAGEMENT \
	-DSUPPORT_HW_RECOVERY

ccflags-$(CONFIG_SGX_540_BUILD_DEBUG) += \
	-DPVR_BUILD_TYPE="\"debug\""  -DDEBUG \
	-DDEBUG_LINUX_MEMORY_ALLOCATIONS \
	-DDEBUG_LINUX_MEM_AREAS \
	-DDEBUG_LINUX_MMAP_AREAS \
	-DDEBUG_BRIDGE_KM \
	-DDEBUG_TRACE_BRIDGE_KM \
	-DPVRSRV_DUMP_MK_TRACE \
	-DSUPPORT_ACTIVE_POWER_MANAGEMENT \
	-DSUPPORT_HW_RECOVERY \
	-DDEBUG_LOG_PATH_TRUNCATE="\"eurasia_km\""

ccflags-$(CONFIG_SGX_PDUMP) += -Idrivers/gpu/pvr/dbgdrv \
	-DPDUMP \
	-DSUPPORT_DBGDRV_EVENT_OBJECTS

pvr_common-y := \
	osfunc.o \
	mutils.o \
	mmap.o \
	module.o \
	pdump.o \
	proc.o \
	pvr_bridge_k.o \
	pvr_debug.o \
	mm.o \
	mutex.o \
	event.o \
	osperproc.o \
	buffer_manager.o \
	devicemem.o \
	deviceclass.o \
	handle.o \
	hash.o \
	metrics.o \
	pvrsrv.o \
	queue.o \
	ra.o \
	resman.o \
	power.o \
	mem.o \
	pdump_common.o \
	bridged_support.o \
	bridged_pvr_bridge.o \
	perproc.o \
	lists.o \
	mem_debug.o \
	osfunc_common.o

pvr540-$(CONFIG_SGX540) := \
	s5pc110/sysconfig.o \
	s5pc110/sysutils.o

sgx-y :=  \
	sgx/bridged_sgx_bridge.o \
	sgx/sgxinit.o \
	sgx/sgxpower.o \
	sgx/sgxreset.o \
	sgx/sgxutils.o \
	sgx/sgxkick.o \
	sgx/sgxtransfer.o \
	sgx/mmu.o \
	sgx/pb.o

s3c_lcd-y := \
	s3c_lcd/s3c_displayclass.o \
	s3c_lcd/s3c_lcd.o

dbgdrv-$(CONFIG_SGX_PDUMP) := \
	dbgdrv/main.o \
	dbgdrv/dbgdriv.o \
	dbgdrv/ioctl.o \
	dbgdrv/handle.o \
	dbgdrv/hostfunc.o \
	dbgdrv/hotkey.o

obj-$(CONFIG_SGX540) := pvr_common.o sgx.o s3c_lcd.o pvr540.o
obj-$(CONFIG_SGX_PDUMP) += dbgdrv.o
