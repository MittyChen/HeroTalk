#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform float nightDegree;

void main(void)
{
	vec4 c = texture2D(CC_Texture0, v_texCoord);
	gl_FragColor.xyz = vec3(nightDegree*c.r , nightDegree*c.g , nightDegree*c.b);
	gl_FragColor.w = c.w;
}