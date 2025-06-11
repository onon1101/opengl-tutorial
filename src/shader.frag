// shader.frag
#version 410 core
in vec2 TexCoord;
out vec4 FragColor;

void main() {
    if (TexCoord.x < 0.001) {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);  // 紅色 debug 區塊
    } else if (TexCoord.x < 0.5) {
        int x = int(gl_FragCoord.x) / 2;
        int y = int(gl_FragCoord.y) / 2;
        if ((x + y) % 2 == 0)
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);  // 強紅色
        else
            FragColor = vec4(0.0, 0.0, 0.0, 1.0);  // 黑色
    } else {
        FragColor = vec4(0.5, 0.0, 0.0, 1.0);      // 中等紅色
    }
}