import pytest
import json

from testsuite.databases import pgsql


# Start the tests via `make test-debug` or `make test-release`


async def test_load_task(service_client):
    data = {
        "name": "Sum",
        "tests": "1 2#3|3 4#7|5 -1#4",
        "time_limit": 1000,
        "memory_limit": 1024,
    }
    headers = {"Content-Type": "application/json"}

    response = await service_client.post(
        "/v1/load", data=json.dumps(data), headers=headers
    )
    assert response.status == 200


async def test_load_task_and_send_ok_solution(service_client):
    data = {
        "name": "Sum",
        "tests": "1 2#3|3 4#7|5 -1#4",
        "time_limit": 1000,
        "memory_limit": 1024,
    }
    headers = {"Content-Type": "application/json"}

    response = await service_client.post(
        "/v1/load", data=json.dumps(data), headers=headers
    )
    assert response.status == 200

    data = {
        "task_id": 1,
        "language": "python",
        "solution": "a, b = map(int, input().split())\nprint(a + b)",
    }

    response = await service_client.post(
        "/v1/submit", data=json.dumps(data), headers=headers
    )
    assert response.status == 200


async def test_load_task_and_send_wa_solution(service_client):
    data = {
        "name": "Sum",
        "tests": "1 2#3|3 4#7|5 -1#4",
        "time_limit": 1000,
        "memory_limit": 1024,
    }
    headers = {"Content-Type": "application/json"}

    response = await service_client.post(
        "/v1/load", data=json.dumps(data), headers=headers
    )
    assert response.status == 200

    data = {
        "task_id": 1,
        "language": "python",
        "solution": "a, b = map(int, input().split())\nprint(a + b + 1)",
    }

    response = await service_client.post(
        "/v1/submit", data=json.dumps(data), headers=headers
    )
    assert response.status == 200
