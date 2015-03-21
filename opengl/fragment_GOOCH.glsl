#version 120
/* Code taken from http://www.ozone3d.net/tutorials/glsl_lighting_phong_p2.php */


/*	Gooch Parameters	*/

float 	gooch_b = 0.55,
	gooch_y = 0.3,
	gooch_alpha = 0.25,
	gooch_beta = 0.5;


varying vec3 N, lightDir, eyeVec;
vec4 	kCool_base = vec4(0, 0, gooch_b, 1),
	kWarm_base = vec4(gooch_y, gooch_y, 0, 1),
	kCool, kWarm, kd;



void main()
{
	kd = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;
	kCool = kCool_base + kd * gooch_alpha;
	kWarm = kWarm_base + kd * gooch_beta;

	vec3 	normal = normalize(N),
	 	L = normalize(lightDir),
		eyeNormalVec = normalize(eyeVec);

	float 	lambertTerm = dot(normal, L),
	 	viewTerm = dot(normal, eyeNormalVec);

	vec4 final_color = kCool * (1-lambertTerm) / 2   +   kWarm * (1+lambertTerm) / 2;


	if (lambertTerm > 0.0)
	{
		//final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;
		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, normal);
		float specular = pow(max(dot(R, E), 0.0), gl_FrontMaterial.shininess);
		final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;
	}

	if (viewTerm < 0.3)
	{
		final_color *= 0;
	}
	gl_FragColor = final_color;
}
