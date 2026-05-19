void *gameLoop(void *data) {
	bool gameRunning = true;
	while (gameRunning) {
		SystemEvent se;
		while (popEvent(&se)) {
			if (se.type == STDIN_FILENO) {
				if (se.data == 27) {
					gameRunning = false;
					atomic_store_explicit(&running, false, memory_order_release);
					wakeEvent();
				}
			}
		}
	}
	printf("game loop over\n");
}


void pushEvent(int type, int data) {
	SystemEvent se;
	se.type = type;
	se.data = data;

	SystemQueue_aqPush(&events, se);
}

bool popEvent(SystemEvent *se) {
	bool ret = SystemQueue_aqPop(&events, se);
	return ret;
}

