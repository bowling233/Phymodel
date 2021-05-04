#version 430 core 
uniform mat4 v_matrix;
uniform mat4 p_matrix;                                                                
                                                                                  
layout (vertices = 3) out;                                                        	// out-patch 的顶点个数，细分控制着色器将会被执行3次
                                                                                  
void main(void)                                                                   
{                                                                                 	// 仅在第一次执行（第一个顶点）时赋值（控制细分程度）
    if (gl_InvocationID == 0)                                                     
    {                                                                             	
        gl_TessLevelInner[0] = 10.0;                                               	// 内部划分5个区域（新增4排顶点，见下图）
        gl_TessLevelOuter[0] = 10.0;                                               	// 左边划分5段
        gl_TessLevelOuter[1] = 10.0;                                               	// 右边划分5段
        gl_TessLevelOuter[2] = 10.0;                                               	// 下边划分5段
    }                                                                             
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;     	// 通常直接将 in-patch 顶点赋给 out-patch 顶点（也可以新建或移除）
}