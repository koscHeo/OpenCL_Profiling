#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>

const char *clu_ErrorString(cl_int err) {
	static const char *errorStrs[] = {
		"CL_SUCCESS",
		"CL_DEVICE_NOT_FOUND",
		"CL_DEVICE_NOT_AVAILABLE",
		"CL_COMPILER_NOT_AVAILABLE",
		"CL_MEM_OBJECT_ALLOCATION_FAILURE",
		"CL_OUT_OF_RESOURCES",
		"CL_OUT_OF_HOST_MEMORY",
		"CL_PROFILING_INFO_NOT_AVAILABLE",
		"CL_MEM_COPY_OVERLAP",
		"CL_IMAGE_FORMAT_MISMATCH",
		"CL_IMAGE_FORMAT_NOT_SUPPORTED",
		"CL_BUILD_PROGRAM_FAILURE",
		"CL_MAP_FAILURE",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"CL_INVALID_VALUE",
		"CL_INVALID_DEVICE_TYPE",
		"CL_INVALID_PLATFORM",
		"CL_INVALID_DEVICE",
		"CL_INVALID_CONTEXT",
		"CL_INVALID_QUEUE_PROPERTIES",
		"CL_INVALID_MEM_OBJECT",
		"CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
		"CL_INVALID_IMAGE_SIZE",
		"CL_INVALID_SAMPLER",
		"CL_INVALID_BINARY",
		"CL_INVALID_BUILD_OPTIONS",
		"CL_INVALID_PROGRAM",
		"CL_INVALID_PROGRAM_EXECUTABLE",
		"CL_INVALID_KERNEL_NAME",
		"CL_INVALID_KERNEL_DEFINITION",
		"CL_INVALID_KERNEL",
		"CL_INVALID_ARG_INDEX",
		"CL_INVALID_ARG_VALUE",
		"CL_INVALID_ARG_SIZE",
		"CL_INVALID_KERNEL_ARGS",
		"CL_INVALID_WORK_DIMENSION",
		"CL_INVALID_WORK_GROUP_SIZE",
		"CL_INVALID_WORK_ITEM_SIZE",
		"CL_INVALID_GLOBAL_OFFSET",
		"CL_INVALID_EVENT_WAIT_LIST",
		"CL_INVALID_EVENT",
		"CL_INVALID_OPERATION",
		"CL_INVALID_GL_OBJECT",
		"CL_INVALID_BUFFER_SIZE",
		"CL_INVALID_MIP_LEVEL",
		"CL_INVALID_GLOBAL_WORK_SIZE",
	};

	int index = -err;
	int errorCount = sizeof(errorStrs) / sizeof(errorStrs[0]);
}



static inline void clu_CheckError(cl_int err, const char *msg) {
	if (err != CL_SUCCESS) {
		fprintf(stderr, "ERROR: %s (%d:%s)\n", msg, err, clu_ErrorString(err));
		exit(EXIT_FAILURE);
	}
}

void opencl_info() {
	cl_int           err_code;

	cl_platform_id  *platforms;
	cl_device_type   device_type;
	cl_uint          num_devices;
	cl_device_id    *devices;

	/* Get OpenCL platforms */
	/* - Get the number of available platforms */
	cl_uint num_platforms;
	err_code = clGetPlatformIDs(0, NULL, &num_platforms);
	clu_CheckError(err_code, "clGetPlatformIDs() for num_platforms");
	if (num_platforms == 0) {
		fprintf(stderr, "No OpenCL platform!\n");
		exit(EXIT_FAILURE);
	}

	/* - Get platform IDs */
	platforms = (cl_platform_id *)malloc(num_platforms*sizeof(cl_platform_id));
	err_code = clGetPlatformIDs(num_platforms, platforms, NULL);
	clu_CheckError(err_code, "clGetPlatformIDs()");

	/* Get platform informations */
	printf("\nNumber of platforms: %u\n\n", num_platforms);
	char tmp_buf[1024];
	cl_uint i = 0;
	for (i = 0; i < num_platforms; i++) {
		printf("platform: %u\n", i);

		err_code = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 1024,
				&tmp_buf, NULL);
		clu_CheckError(err_code, "clGetPlatformInfo() for CL_PLATFORM_NAME");
		printf("- CL_PLATFORM_NAME      : %s\n", tmp_buf);
		err_code = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 1024,
				&tmp_buf, NULL);
		clu_CheckError(err_code, "clGetPlatformInfo() for CL_PLATFORM_VENDOR");
		printf("- CL_PLATFORM_VENDOR    : %s\n", tmp_buf);

		err_code = clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, 1024,
				&tmp_buf, NULL);
		clu_CheckError(err_code, "clGetPlatformInfo() for CL_PLATFORM_PROFILE");
		printf("- CL_PLATFORM_PROFILE   : %s\n", tmp_buf);

		err_code = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 1024,
				&tmp_buf, NULL);
		clu_CheckError(err_code, "clGetPlatformInfo() for CL_PLATFORM_VERSION");
		printf("- CL_PLATFORM_VERSION   : %s\n", tmp_buf);

		err_code = clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 1024,
				&tmp_buf, NULL);
		clu_CheckError(err_code,"clGetPlatformInfo() for CL_PLATFORM_EXTENSIONS");
		printf("- CL_PLATFORM_EXTENSIONS: %s\n", tmp_buf);
		printf("\n");
		/* Get the number of devices */
		err_code = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL,
				&num_devices);
		clu_CheckError(err_code, "clGetDeviceIDs for num_devices");
		if (num_devices == 0) {
			fprintf(stderr, "No OpenCL device in this platform!\n");
			exit(EXIT_FAILURE);
		}
		printf("Number of devices: %u\n", num_devices);

		/* Get the default device */
		cl_device_id default_device;
		cl_uint num_defaults;
		err_code = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_DEFAULT,
				1, &default_device, &num_defaults);
		clu_CheckError(err_code, "clGetDeviceIDs() for CL_DEVICE_TYPE_DEFAULT");
		if (num_defaults != 1) printf("- # of default devices: %u\n", num_defaults);

		/* Get device IDs */
		devices = (cl_device_id *)malloc(num_devices * sizeof(cl_device_id));
		err_code = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, num_devices,
				devices, NULL);
		clu_CheckError(err_code, "clGetDeviceIDs()");
		
		cl_uint k = 0;
		for (k = 0; k < num_devices; k++) {
			printf("device: %u (", k);
			err_code = clGetDeviceInfo(devices[k], CL_DEVICE_TYPE,
					sizeof(cl_device_type), &device_type, NULL);
			if (device_type & CL_DEVICE_TYPE_CPU)
				printf("CL_DEVICE_TYPE_CPU");
			if (device_type & CL_DEVICE_TYPE_GPU)
				printf("CL_DEVICE_TYPE_GPU");
			if (device_type & CL_DEVICE_TYPE_ACCELERATOR)
				printf("CL_DEVICE_TYPE_ACCELERATOR");
			if (device_type & CL_DEVICE_TYPE_DEFAULT)
				printf("CL_DEVICE_TYPE_DEFAULT");
			printf(")");
			if (default_device == devices[k]) printf(" default");
			printf("\n");

			err_code = clGetDeviceInfo(devices[k], CL_DEVICE_NAME,
					1024, tmp_buf, NULL);
			printf(" - CL_DEVICE_NAME                         : %s\n", tmp_buf);
			err_code = clGetDeviceInfo(devices[k], CL_DEVICE_VENDOR,
					1024, tmp_buf, NULL);
			printf(" - CL_DEVICE_VENDOR                       : %s\n", tmp_buf);

			err_code = clGetDeviceInfo(devices[k], CL_DRIVER_VERSION,
					1024, tmp_buf, NULL);
			printf(" - CL_DRIVER_VERSION                      : %s\n", tmp_buf);

			err_code = clGetDeviceInfo(devices[k], CL_DEVICE_PROFILE,
					1024, tmp_buf, NULL);
			printf(" - CL_DEVICE_PROFILE                      : %s\n", tmp_buf);

			err_code = clGetDeviceInfo(devices[k], CL_DEVICE_VERSION,
					1024, tmp_buf, NULL);
			printf(" - CL_DEVICE_VERSION                      : %s\n", tmp_buf);

			err_code = clGetDeviceInfo(devices[k], CL_DEVICE_EXTENSIONS,
					1024, tmp_buf, NULL);
			printf(" - CL_DEVICE_EXTENSIONS                   : %s\n", tmp_buf);
			cl_uint vendor_id;
			err_code = clGetDeviceInfo(devices[k], CL_DEVICE_VENDOR_ID,
					sizeof(cl_uint), &vendor_id, NULL);
			printf(" - CL_DEVICE_VENDOR_ID                    : %u\n", vendor_id);

			cl_uint max_cus;
			err_code = clGetDeviceInfo(devices[k], CL_DEVICE_MAX_COMPUTE_UNITS,
					sizeof(cl_uint), &max_cus, NULL);
			printf(" - CL_DEVICE_MAX_COMPUTE_UNITS            : %u\n", max_cus);

			cl_uint max_widim;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
					sizeof(cl_uint), &max_widim, NULL);
			printf(" - CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS     : %u\n", max_widim);

			size_t max_wisizes[3];
			err_code = clGetDeviceInfo(devices[k], CL_DEVICE_MAX_WORK_ITEM_SIZES,
					sizeof(max_wisizes), &max_wisizes, NULL);
			printf(" - CL_DEVICE_MAX_WORK_ITEM_SIZES          : (%lu, %lu, %lu)\n",
					max_wisizes[0], max_wisizes[1], max_wisizes[2]);
			size_t max_wgsize;
			err_code = clGetDeviceInfo(devices[k], CL_DEVICE_MAX_WORK_GROUP_SIZE,
					sizeof(size_t), &max_wgsize, NULL);
			printf(" - CL_DEVICE_MAX_WORK_GROUP_SIZE          : %lu\n", max_wgsize);

			cl_uint vec_size;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,
					sizeof(cl_uint), &vec_size, NULL);
			printf(" - CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR  : %u\n", vec_size);
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,
					sizeof(cl_uint), &vec_size, NULL);
			printf(" - CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT : %u\n", vec_size);
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
					sizeof(cl_uint), &vec_size, NULL);
			printf(" - CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT   : %u\n", vec_size);
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,
					sizeof(cl_uint), &vec_size, NULL);
			printf(" - CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG  : %u\n", vec_size);
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
					sizeof(cl_uint), &vec_size, NULL);
			printf(" - CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT : %u\n", vec_size);
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,
					sizeof(cl_uint), &vec_size, NULL);
			printf(" - CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE: %u\n", vec_size);

			cl_uint max_clock;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_MAX_CLOCK_FREQUENCY,
					sizeof(cl_uint), &max_clock, NULL);
			printf(" - CL_DEVICE_MAX_CLOCK_FREQUENCY          : %u\n", max_clock);

			cl_uint addr_bits;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_ADDRESS_BITS,
					sizeof(cl_uint), &addr_bits, NULL);
			printf(" - CL_DEVICE_ADDRESS_BITS                 : %u\n", addr_bits);
			cl_ulong max_alloc;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_MAX_MEM_ALLOC_SIZE,
					sizeof(cl_ulong), &max_alloc, NULL);
			printf(" - CL_DEVICE_MAX_MEM_ALLOC_SIZE           : %lu\n", max_alloc);

			cl_bool image_support;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_IMAGE_SUPPORT,
					sizeof(cl_bool), &image_support, NULL);
			printf(" - CL_DEVICE_IMAGE_SUPPORT                : %s\n",
					(image_support == CL_TRUE ? "CL_TRUE" : "CL_FALSE"));

			cl_uint max_rimage;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_MAX_READ_IMAGE_ARGS,
					sizeof(cl_uint), &max_rimage, NULL);
			printf(" - CL_DEVICE_MAX_READ_IMAGE_ARGS          : %u\n", max_rimage);

			cl_uint max_wimage;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_MAX_WRITE_IMAGE_ARGS,
					sizeof(cl_uint), &max_wimage, NULL);
			printf(" - CL_DEVICE_MAX_WRITE_IMAGE_ARGS         : %u\n", max_wimage);
			size_t image2d_width;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_IMAGE2D_MAX_WIDTH,
					sizeof(size_t), &image2d_width, NULL);
			printf(" - CL_DEVICE_IMAGE2D_MAX_WIDTH            : %lu\n",
					image2d_width);

			size_t image2d_height;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_IMAGE2D_MAX_HEIGHT,
					sizeof(size_t), &image2d_height, NULL);
			printf(" - CL_DEVICE_IMAGE2D_MAX_HEIGHT           : %lu\n",
					image2d_height);

			size_t image3d_width;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_IMAGE3D_MAX_WIDTH,
					sizeof(size_t), &image3d_width, NULL);
			printf(" - CL_DEVICE_IMAGE3D_MAX_WIDTH            : %lu\n",
					image3d_width);
			size_t image3d_height;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_IMAGE3D_MAX_HEIGHT,
					sizeof(size_t), &image3d_height, NULL);
			printf(" - CL_DEVICE_IMAGE3D_MAX_HEIGHT           : %lu\n",
					image3d_height);

			size_t image3d_depth;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_IMAGE3D_MAX_DEPTH,
					sizeof(size_t), &image3d_depth, NULL);
			printf(" - CL_DEVICE_IMAGE3D_MAX_DEPTH            : %lu\n",
					image3d_depth);

			cl_uint max_samplers;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_MAX_SAMPLERS,
					sizeof(cl_uint), &max_samplers, NULL);
			printf(" - CL_DEVICE_MAX_SAMPLERS                 : %u\n",
					max_samplers);
			size_t max_param;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_MAX_PARAMETER_SIZE,
					sizeof(size_t), &max_param, NULL);
			printf(" - CL_DEVICE_MAX_PARAMETER_SIZE           : %lu\n", max_param);

			cl_uint base_align;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_MEM_BASE_ADDR_ALIGN,
					sizeof(cl_uint), &base_align, NULL);
			printf(" - CL_DEVICE_MEM_BASE_ADDR_ALIGN          : %u\n", base_align);

			cl_uint type_align;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE,
					sizeof(cl_uint), &type_align, NULL);
			printf(" - CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE     : %u\n", type_align);

			cl_device_fp_config fp_config;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_SINGLE_FP_CONFIG,
					sizeof(cl_device_fp_config),
					&fp_config, NULL);
			printf(" - CL_DEVICE_SINGLE_FP_CONFIG             : ");
			if (fp_config & CL_FP_DENORM)
				printf("CL_FP_DENORM ");
			if (fp_config & CL_FP_INF_NAN)
				printf("CL_FP_INF_NAN ");
			if (fp_config & CL_FP_ROUND_TO_NEAREST)
				printf("CL_FP_ROUND_TO_NEAREST ");
			if (fp_config & CL_FP_ROUND_TO_ZERO)
				printf("CL_FP_ROUND_TO_ZERO ");
			if (fp_config & CL_FP_ROUND_TO_INF)
				printf("CL_FP_ROUND_TO_INF ");
			if (fp_config & CL_FP_FMA)
				printf("CL_FP_FMA ");
			printf("\n");

			cl_device_mem_cache_type gcache_type;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
					sizeof(cl_device_mem_cache_type),
					&gcache_type, NULL);
			printf(" - CL_DEVICE_GLOBAL_MEM_CACHE_TYPE        : ");
			switch (gcache_type) {
				case CL_NONE:             printf("CL_NONE\n"); break;
				case CL_READ_ONLY_CACHE:  printf("CL_READ_ONLY_CACHE\n"); break;
				case CL_READ_WRITE_CACHE: printf("CL_READ_WRTIE_CACHE\n"); break;
				default: printf("Unknown cache\n");
			}

			cl_uint cacheline_size;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
					sizeof(cl_ulong),
					&cacheline_size, NULL);
			printf(" - CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE    : %u\n",
					cacheline_size);

			cl_ulong cache_size;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
					sizeof(cl_ulong),
					&cache_size, NULL);
			printf(" - CL_DEVICE_GLOBAL_MEM_CACHE_SIZE        : %lu\n",
					cache_size);
			cl_ulong gmem_size;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_GLOBAL_MEM_SIZE,
					sizeof(cl_ulong),
					&gmem_size, NULL);
			printf(" - CL_DEVICE_GLOBAL_MEM_SIZE              : %lu\n", gmem_size);

			cl_ulong cbuf_size;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
					sizeof(cl_ulong),
					&cbuf_size, NULL);
			printf(" - CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE     : %lu\n", cbuf_size);

			cl_uint max_const_args;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_MAX_CONSTANT_ARGS,
					sizeof(cl_uint),
					&max_const_args, NULL);
			cl_device_local_mem_type local_mem_type;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_LOCAL_MEM_TYPE,
					sizeof(cl_device_local_mem_type),
					&local_mem_type, NULL);
			printf(" - CL_DEVICE_LOCAL_MEM_TYPE               : %s\n",
					(local_mem_type == CL_LOCAL ? "CL_LOCAL" : "CL_GLOBAL"));

			cl_ulong lmem_size;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_LOCAL_MEM_SIZE,
					sizeof(cl_ulong),
					&lmem_size, NULL);
			printf(" - CL_DEVICE_LOCAL_MEM_SIZE               : %lu\n", lmem_size);

			cl_bool error_cor;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_ERROR_CORRECTION_SUPPORT,
					sizeof(cl_bool), &error_cor, NULL);
			printf(" - CL_DEVICE_ERROR_CORRECTION_SUPPORT     : %s\n",
					(error_cor == CL_TRUE ? "CL_TRUE" : "CL_FALSE"));
			size_t timer_res;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_PROFILING_TIMER_RESOLUTION,
					sizeof(size_t), &timer_res, NULL);
			printf(" - CL_DEVICE_PROFILING_TIMER_RESOLUTION   : %lu ns\n", timer_res);

			cl_bool endian_little;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_ENDIAN_LITTLE,
					sizeof(cl_bool), &endian_little, NULL);
			printf(" - CL_DEVICE_ENDIAN_LITTLE                : %s\n",
					(endian_little == CL_TRUE ? "CL_TRUE" : "CL_FALSE"));

			cl_bool dev_available;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_AVAILABLE,
					sizeof(cl_bool),
					&dev_available, NULL);
			printf(" - CL_DEVICE_AVAILABLE                    : %s\n",
					(dev_available == CL_TRUE ? "CL_TRUE" : "CL_FALSE"));
			cl_bool compiler_available;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_COMPILER_AVAILABLE,
					sizeof(cl_bool),
					&compiler_available, NULL);
			printf(" - CL_DEVICE_COMPILER_AVAILABLE           : %s\n",
					(compiler_available == CL_TRUE ? "CL_TRUE" : "CL_FALSE"));

			cl_device_exec_capabilities exec_cap;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_EXECUTION_CAPABILITIES,
					sizeof(cl_device_exec_capabilities),
					&exec_cap, NULL);
			printf(" - CL_DEVICE_EXECUTION_CAPABILITIES       : ");
			if (exec_cap & CL_EXEC_KERNEL)
				printf("CL_EXEC_KERNEL ");
			if (exec_cap & CL_EXEC_NATIVE_KERNEL)
				printf("CL_EXEC_NATIVE_KERNEL");
			printf("\n");
			cl_command_queue_properties cmd_q_prop;
			err_code = clGetDeviceInfo(devices[k],
					CL_DEVICE_QUEUE_PROPERTIES,
					sizeof(cl_command_queue_properties),
					&cmd_q_prop, NULL);
			printf(" - CL_DEVICE_QUEUE_PROPERTIES             : ");
			if (cmd_q_prop & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)
				printf("CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE ");
			if (cmd_q_prop & CL_QUEUE_PROFILING_ENABLE)
				printf("CL_QUEUE_PROFILING_ENABLE ");
			printf("\n");

			printf("\n");
		}
		free(devices);

		printf("\n");
	}

	free(platforms);
}

int main(int argc, char **argv) {
	opencl_info();
	fflush(stdout);
	fflush(stderr);

	return EXIT_SUCCESS;
}
