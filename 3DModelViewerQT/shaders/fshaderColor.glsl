#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform vec4 color;
varying vec2 v_texcoord;

void main()
{
	gl_FragColor = color;
	if(v_texcoord.x < 0.1)
	{
		gl_FragColor = color * 0.5;
	}
}
