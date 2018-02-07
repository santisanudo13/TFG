int main(){
	int init;

	init =camera_init(0x01, 3);

	printf("init: %d\n",init);
	int objetos;

	char buffer[8];
	camera_device_id(buffer);

	printf("Version: %c%c%c%c%c%c%c%c\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7]);

	camera_start_Tracking();
	sleep(1);
	objetos=camera_objects_detected();

	printf("Objetos %d\n",objetos);
	//camera_reset();
	return 0;
}
