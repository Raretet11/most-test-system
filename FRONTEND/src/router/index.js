// src/router/index.js
import { createRouter, createWebHistory } from 'vue-router';
import TaskArchive from '../components/TaskArchive.vue';
import TaskDetail from '../components/TaskDetail.vue';

const routes = [
  {
    path: '/',
    name: 'TaskArchive',
    component: TaskArchive
  },
  {
    path: '/task/:taskNumber',
    name: 'TaskDetail',
    component: TaskDetail
  }
];

const router = createRouter({
  history: createWebHistory(),
  routes
});

export default router;