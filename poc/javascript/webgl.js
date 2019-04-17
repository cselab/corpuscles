main()

function main()
{
	var c, g, info, buffers

	c = document.querySelector('#c')
	g = c.getContext('webgl')
	info = initInof(g)
	buffers= initBuffers(g)
	draw(g, info.program, info, buffers)
}

function initInof(g)
{
	var v, f, info, program

	v = `
		attribute vec4 Vertex;
		uniform mat4 View, Projection;
		void main() {
			gl_Position = Projection*View*Vertex;
		}
	`
	f = `
		void main() {
			gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
	`
	program = initProgram(g, v, f)
	info = {
		program: program,
		Vertex: g.getAttribLocation(program, 'Vertex'),
		Projection: g.getUniformLocation(program, 'Projection'),
		View: g.getUniformLocation(program, 'View')
	}
	return info
}

function initBuffers(g)
{
	var b, pos
	b = g.createBuffer()
	g.bindBuffer(g.ARRAY_BUFFER, b)
	pos = [
		 1.0,	1.5,
		-1.2,	1.0,
		 1.0, -1.0,
		-1.0, -1.0,
	]
	g.bufferData(g.ARRAY_BUFFER, new Float32Array(pos), g.STATIC_DRAW)
	return { position: b }
}

function matrix_perspective(out, fovy, aspect, near, far)
{
	var f, nf
	f = 1.0 / Math.tan(fovy / 2)
	nf = 1 / (near - far)
	out[0] = f / aspect
	out[1] = 0
	out[2] = 0
	out[3] = 0
	out[4] = 0
	out[5] = f
	out[6] = 0
	out[7] = 0
	out[8] = 0
	out[9] = 0
	out[10] = (far + near) * nf
	out[11] = -1
	out[12] = 0
	out[13] = 0
	out[14] = 2 * far * near * nf
	out[15] = 0
	return out
}

function matrix_create()
{
	var out
	out = new Array(16)
	out[0] = 1
	out[1] = 0
	out[2] = 0
	out[3] = 0
	out[4] = 0
	out[5] = 1
	out[6] = 0
	out[7] = 0
	out[8] = 0
	out[9] = 0
	out[10] = 1
	out[11] = 0
	out[12] = 0
	out[13] = 0
	out[14] = 0
	out[15] = 1
	return out
}

function matrix_translate(out, a, v) {
	var x = v[0], y = v[1], z = v[2]
	out[12] = a[0] * x + a[4] * y + a[8] * z + a[12]
	out[13] = a[1] * x + a[5] * y + a[9] * z + a[13]
	out[14] = a[2] * x + a[6] * y + a[10] * z + a[14]
	out[15] = a[3] * x + a[7] * y + a[11] * z + a[15]

	return out
}

function draw(g, program, info, buffers)
{
	var fov, aspect, zNear, zFar, View, Projection
	fov = 45*Math.PI/180
	aspect = g.canvas.clientWidth/g.canvas.clientHeight
	zNear = 0.1
	zFar = 100.0
	Projection = matrix_create()
	matrix_perspective(Projection, fov, aspect, zNear, zFar)
	View = matrix_create()
	matrix_translate(View, View, [0, 0, -4])								 
	{
		const numComponents = 2
		const type = g.FLOAT
		const normalize = false
		const stride = 0
		const offset = 0
		g.bindBuffer(g.ARRAY_BUFFER, buffers.position)
		g.vertexAttribPointer(
				info.Vertex,
				numComponents,
				type,
				normalize,
				stride,
				offset)
		g.enableVertexAttribArray(
				info.Vertex)
	}
	g.useProgram(program)
	g.uniformMatrix4fv(
			info.Projection,
			false,
			Projection)
	g.uniformMatrix4fv(
			info.View,
			false,
			View)
	const offset = 0
	const vertexCount = 4
	g.drawArrays(g.TRIANGLE_STRIP, offset, vertexCount)
}

function initProgram(g, v, f)
{
	var p
	v = loadShader(g, g.VERTEX_SHADER, v)
	f = loadShader(g, g.FRAGMENT_SHADER, f)
	p = g.createProgram()
	g.attachShader(p, v)
	g.attachShader(p, f)
	g.linkProgram(p)
	if (!g.getProgramParameter(p, g.LINK_STATUS)) {
		alert('Unable to initialize the shader program: ' + g.getProgramInfoLog(p))
		return null
	}
	return p
}

function loadShader(g, type, source)
{
	var shader
	shader = g.createShader(type)
	g.shaderSource(shader, source)
	g.compileShader(shader)
	if (!g.getShaderParameter(shader, g.COMPILE_STATUS)) {
		alert('An error occurred compiling the shaders: ' + g.getShaderInfoLog(shader))
		g.deleteShader(shader)
		return null
	}
	return shader
}
