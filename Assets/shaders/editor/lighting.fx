float3 lightingP(float3 pos, float3 base, float3 normal, float3 lightPos, float3 _camPos ){

	// Lighting vectors
	float3 lVec = (lightPos - pos);
	float3 lightVec = normalize(lVec);
	float3 viewVec = normalize(pos - _camPos);

	// Attenution that falls off to zero at light radius
	const float a = 0.15;
	float atten = 2 * saturate((1.0 + a) / (1.0 + (1.0 / a) * dot(lVec, lVec)) - a);
//  atten = 1.0;

	// Lighting
	float diffuse = saturate(dot(lightVec, normal));
	float specular = 0.2 * pow(saturate(dot(reflect(-viewVec, normal), lightVec)), 10.0);

//return (diffuse * base + specular);
	//return atten * (diffuse * base + specular);
return diffuse * (base );
}