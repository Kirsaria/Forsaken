void main() {
    vec2 pixelCoord = gl_FragCoord.xy;

    vec3 startColor = vec3(1.0, 0.0, 0.0); 
    vec3 endColor = vec3(1.0, 1.0, 0.0);   

    float progress = pixelCoord.x / resolution.x;
  
    vec3 finalColor = mix(startColor, endColor, progress);

    gl_FragColor = vec4(finalColor, 1.0);
}