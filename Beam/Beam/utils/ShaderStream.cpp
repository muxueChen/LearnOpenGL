//
//  ShaderStream.cpp
//  Shaders
//
//  Created by 陈学明 on 2021/7/2.
//

#include "ShaderStream.hpp"

ShaderStream:: ShaderStream() {
    // 顶点着色器
    vertextCode = vertexStream();
    // 片段着色器代码
    fragmentCode = fragmentStream();
}

char * ShaderStream:: vertexStream() {
    return SHADER_SRC(
        \#version 330 core\n
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTexCoord;

      out vec2 vProgressRange;
      out vec3 vColor;

      vec2 fun(in vec2 p0, in vec2 p1, in vec2 p2, in vec2 p3, in float t){
          float tt = (1.0 - t) * (1.0 -t);
          return tt * (1.0 -t) *p0 +
          3.0 * t * tt * p1 +
          3.0 * t *t *(1.0 -t) *p2 + t *t *t *p3;
      }

      // 三阶贝塞尔
      vec2 fun2(in vec2 p0, in vec2 p1, in vec2 p2, in vec2 p3, in float t)
      {
          vec2 q0 = mix(p0, p1, t);
          vec2 q1 = mix(p1, p2, t);
          vec2 q2 = mix(p2, p3, t);

          vec2 r0 = mix(q0, q1, t);
          vec2 r1 = mix(q1, q2, t);

          return mix(r0, r1, t);
      }

      // 二阶贝塞尔
      vec2 fun3(in vec2 p0, in vec2 p1, in vec2 p2, in vec2 p3, in float t)
      {

          float tt = (1.0 - t) * (1.0 -t);

          return tt * p0 + 2.0 * t * (1.0 -t) * p1 + t * t * p2;

      }


      //(1-t)^2P0 + 2(1-t)tP1 + t^2*P2
      //(1-t)^3P0 + 3(1-t)^2tP1 + 3(1-t)t^2P2 + t^3*P3
      void main () {

          vec4 uStartEndData = PREFIX_DRAW_INSTANCE(uStartEndData);
          vec4 uControlData = PREFIX_DRAW_INSTANCE(uControlData);
          float uRadius = PREFIX(uRadius);
          float uProgress = PREFIX(uProgress);
          vec2 uProgressRange = PREFIX_DRAW_INSTANCE(uProgressRange);
          float uProgressEnd = PREFIX(uProgressEnd);
          float uZoomStartNode = PREFIX(uZoomStartNode);
          float uZoomEndNode = PREFIX(uZoomEndNode);
          vec4 uColor = PREFIX_DRAW_INSTANCE(uColor);
          float uPointCount = float(PREFIX(uPointCount));
          vColor = uColor.rgb;

          vProgressRange = uProgressRange;

          vec2 viewSize = vec2(PREFIX(uViewSizeWidth),PREFIX(uViewSizeHeight));

          vec4 pos;
          pos.w = 1.0;
          pos.z = 0.0;

          vec2 p0 = uStartEndData.xy;
          vec2 p3 = uStartEndData.zw;

          vec2 p1 = uControlData.xy;
          vec2 p2 = uControlData.zw;
          
          float standard = viewSize.y;
          float minStandard = standard*smoothstep(0.0,1.0,(min(iResolutionOrig.y,standard)/standard));

          float t = position.x/uPointCount;
          tPos = t;
          vec2 point = fun2(p0, p1, p2, p3, t)*vec2(1.0,-1.0);

          float tempT = uProgressRange.x+(uProgressRange.y-uProgressRange.x)*t;
          float scale = smoothstep(uProgress,uProgressEnd,tempT)+step(1.0,uProgressEnd)*(1.0-step(uProgressEnd-1.0,tempT))*smoothstep(uProgress,uProgressEnd,1.0+tempT);
          float radius = max(mix(1.0,pow(scale*1.2,0.8),uZoomStartNode)*mix(1.0,pow((1.0-scale)*1.2,0.8),uZoomEndNode)*uRadius/standard,1.5/minStandard);
          pos.xy = point+radius*texcoord.xy*vec2(iResolutionOrig.y/iResolutionOrig.x,1.0);
          vUV = texcoord.xy;
          gl_Position = pos;
          tc = vec2(0.5)+vec2(pos.x,-pos.y)*0.5;
      }
    );
}

char * ShaderStream:: fragmentStream() {
    return SHADER_SRC(
        \#version 330 core\n
        out vec4 FragColor;
        in vec2 TexCoord;
        uniform sampler2D texture1;
        uniform sampler2D texture2;

        void main() {
            // 修改笑脸看的方向
            vec2 faceCoord = TexCoord;
            FragColor = mix(texture(texture1, TexCoord), texture(texture2, faceCoord), 0.2);
        }
    );
}
