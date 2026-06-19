linkedList *ActorList = 0;

void makeActorList() {
	ActorList = makeList();
}

void addActor(Actor *a) {
	addToList(&ActorList, a);
}

void removeActor(Actor *a) {
	removeFromList(&ActorList, a);
}

void deleteActorList() {
	deleteList(&ActorList, deleteActor);
}

void actorListDo() {
	linkedList *cur = ActorList;
	linkedList *pre = cur;
	int count = 0;
	while (cur) {
		if (cur->data) {
			Actor *a = cur->data;
			if (a->deleteMe) {
				linkedList *tmp = cur;
				deleteActor(a);
				cur->data = 0;
				if (pre == cur) {
					ActorList = cur->next;
				} else {
					pre->next = cur->next;
				}
				cur = cur->next;
				free(tmp);
				if (cur == 0 && count == 0) {
					free(ActorList);
					ActorList = 0;
					break;
				}
				count++;
				continue;
			}
			if (a->active) {
				doActions(a);
			}
			count++;
		}
		pre = cur;
		cur = cur->next;
	}
}

