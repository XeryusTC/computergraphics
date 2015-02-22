#version 120

varying vec3 N, lightDir, eyeVec;

void main()
{
	vec4 final_color = (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) +
		(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);

	vec3 normal = normalize(N);
	vec3 L = normalize(lightDir);

	float lambertTerm = dot(normal, L);

	if (lambertTerm > 0.0)
	{
		final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;
		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, normal);
		float specular = pow(max(dot(R, E), 0.0), gl_FrontMaterial.shininess);
		final_color += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;
	}
	gl_FragColor = final_color;
}
