#version 460 core \n
		in vec2 vertexAttribute;
		in vec4 colorAttribute;
		out vec4 color;
		
		void main() {
			color = colorAttribute;
			gl_Position = vec4(vertexAttribute, 0.0, 1.0);
		}