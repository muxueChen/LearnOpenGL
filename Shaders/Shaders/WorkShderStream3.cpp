//
//  WorkShderStream3.cpp
//  Shaders
//
//  Created by 陈学明 on 2021/7/3.
//

#include "WorkShderStream3.hpp"

WorkShderStream3:: WorkShderStream3() {
    // 顶点着色器
    vertextCode = vertexStream();
    // 片段着色器代码
    fragmentCode = fragmentStream();
}

char * WorkShderStream3:: vertexStream() {
    return SHADER_SRC(
                      \#version 330 core\n
                      layout (location = 0) in vec3 aPos;
                      out vec3 ourColor;
                      void main() {
                          gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);
                          ourColor = aPos;
                      }
            );
}

char * WorkShderStream3:: fragmentStream() {
    return SHADER_SRC(
                      \#version 330 core\n
                      out vec4 FragColor;
                      in vec3 ourColor;
                      void main() {
                          FragColor = vec4(ourColor, 1.0);// 因为顶点坐标的取值范围在 -1 ～ 1 之间，而颜色分量的取值范围在0～1之间，当顶点位于左下角时，x和y均小于0，且z等于0，将坐标转化成rgb色值，得到对应的 r和g小于0，且b等于0，所以片段显示黑色。
                      }
    );
}
