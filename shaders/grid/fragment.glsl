#version 330 core
in vec2 pos;

out vec4 color;

void main()
{
  // fract is fractional part of float
  // fract(3.1415) = 0.1415 = 3.1415 - floor(3.1415)
  vec2 cell_pos = fract(pos);

  // you can use smoothstep for smooth border
  vec2 border = step(0.01, cell_pos) * (1.0 - step(0.99, cell_pos));
  float is_not_border = border.x * border.y;

  color = vec4(vec3(is_not_border*0.3 + 0.4), 1.0);
  // return;


  // LITTLE BONUS
  // Draw little squares in each cell that indicate cell position in binary notation

  ivec2 cell_i = ivec2(pos);
  ivec2 bits_lines = ivec2(1);
  float c = 1.0 / 11.0; // 11 = 8 bits + 3 offset; oobbbbbbbbo
  bits_lines = ivec2(step(c+c, cell_pos) * (1.0 - step(1.0-c, cell_pos)));
  bits_lines &= ivec2(step(c, cell_pos.yx) * (1.0 - step(c+c, cell_pos.yx)));
  int bit_area = bits_lines.x | bits_lines.y;

  ivec2 bit_i = ivec2((cell_pos-c-c)/c); // bit index [0; 8]
  ivec2 bits_col = bits_lines & (cell_i >> bit_i) & 1; // if nth bit is 1
  int bit_col = bits_col.x | bits_col.y; // if prev condition is true for x or y
  ivec2 odds_col = bits_lines & bit_i & 1; // if nth bit is odd
  int odd_col = odds_col.x | odds_col.y; // if prev condition is true for x or y
  odd_col &= ~bit_col;

  vec3 bonus_col = vec3(-0.05*bit_area + 0.1*bit_col + 0.03*odd_col);

  color = vec4(vec3(is_not_border*0.3 + 0.4 + bonus_col), 1.0);
}