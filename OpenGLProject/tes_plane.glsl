#version 430 core                                                                
uniform mat4 v_matrix;
uniform mat4 p_matrix;                                                         
layout (triangles, equal_spacing, cw) in;                                        	 // 指定图元生成域、细分坐标空间、图元的面朝向 
		//"layout (triangles, equal_spacing, cw, point_mode) in;                             // 输出点模式
                                                                                 
void main(void)                                                                  
{                                                                                  
    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +                      	// gl_TessCoord：细分后的新增坐标（插值比例）
                  (gl_TessCoord.y * gl_in[1].gl_Position) +                      	// 根据 input-patch 生成输出顶点的位置
                  (gl_TessCoord.z * gl_in[2].gl_Position);                         // 每个细分坐标都会让 TES 的执行一次
}                       