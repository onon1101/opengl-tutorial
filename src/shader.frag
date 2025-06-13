#version 410 core

out vec4 FragColor;
in vec3 ourColor;

void main()
{
    // 根據顏色強度調整透明度
    float intensity = dot(ourColor, vec3(0.299, 0.587, 0.114)); // 模擬亮度
    float alpha = smoothstep(0.1, 1.0, intensity);

    // 根據顏色與位置模擬發光邊緣衰退效果
    vec2 center = vec2(0.5, 0.5); // 中心位置（可根據需要調整）
    vec2 pos = gl_FragCoord.xy / vec2(800.0, 600.0); // 螢幕座標歸一化
    float dist = distance(pos, center);
    float glow = exp(-dist * 10.0); // 衰減模擬

    vec3 finalColor = mix(vec3(0.0), ourColor, glow);
    FragColor = vec4(finalColor, alpha);
}