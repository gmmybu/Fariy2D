texture_ucolor = {
  vertexshader =
[[#version 330
layout(location=0) in vec4 a_position;
layout(location=1) in vec2 a_textureCoord;
uniform mat4 MVP;
varying vec2 v_texCoord;
void main()
{
  gl_Position = MVP * a_position;
  v_texCoord  = a_textureCoord;
}
]],

  fragshader =
[[uniform sampler2D texture;
uniform vec4 color;
varying vec2 v_texCoord;
void main()
{
  gl_FragDepth = gl_FragCoord.z;
  gl_FragColor = texture2D(texture, v_texCoord) * color;
}
]]
}

position_ucolor = {
  vertexshader = 
[[#version 330
layout(location=0) in vec4 a_position;
uniform mat4 MVP;
void main()
{
  gl_Position = MVP * a_position;
}
]],

  fragshader = 
[[uniform vec4 color;
void main()
{
  gl_FragDepth = gl_FragCoord.z;
  gl_FragColor = color;
}
]]

}

