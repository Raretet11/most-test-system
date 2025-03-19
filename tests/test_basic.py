import pytest
import json

from testsuite.databases import pgsql


# Start the tests via `make test-debug` or `make test-release`


async def test_load_task(service_client):
    data = {
        "name": "Sum",
        "tests": "1 2#3|3 4#7|5 -1#4",
        "legend": "Sum of two numbers",
        "time_limit": 1000,
        "memory_limit": 1024,
    }
    headers = {"Content-Type": "application/json"}

    response = await service_client.post(
        "/api/tasks/load", data=json.dumps(data), headers=headers
    )
    assert response.status == 200


async def test_load_task_and_send_ok_solution(service_client):
    data = {
        "name": "Sum",
        "tests": "1 2#3|3 4#7|5 -1#4",
        "legend": "Sum of two numbers",
        "time_limit": 1000,
        "memory_limit": 1024,
    }
    headers = {"Content-Type": "application/json"}

    response = await service_client.post(
        "/api/tasks/load", data=json.dumps(data), headers=headers
    )
    assert response.status == 200

    data = {
        "task_id": 1,
        "language": "python",
        "solution": "a, b = map(int, input().split())\nprint(a + b)",
    }

    response = await service_client.post(
        "/api/solutions/submit", data=json.dumps(data), headers=headers
    )
    response_json = response.json()
    assert response_json["verdict"] == "Accepted!"
    assert response.status == 200


async def test_load_task_and_send_wa_solution(service_client):
    data = {
        "name": "Sum",
        "tests": "1 2#3|3 4#7|5 -1#4",
        "legend": "Sum of two numbers",
        "time_limit": 1000,
        "memory_limit": 1024,
    }
    headers = {"Content-Type": "application/json"}

    response = await service_client.post(
        "/api/tasks/load", data=json.dumps(data), headers=headers
    )
    assert response.status == 200

    data = {
        "task_id": 1,
        "language": "python",
        "solution": "a, b = map(int, input().split())\nprint(a + b + 1)",
    }

    response = await service_client.post(
        "/api/solutions/submit", data=json.dumps(data), headers=headers
    )

    response_json = response.json()
    assert response_json["verdict"] == "Wrong answer!"
    assert response.status == 200


async def test_load_task_and_send_compilation_error_solution(service_client):
    data = {
        "name": "Sum",
        "tests": "1 2#3|3 4#7|5 -1#4",
        "legend": "Sum of two numbers",
        "time_limit": 1000,
        "memory_limit": 1024,
    }
    headers = {"Content-Type": "application/json"}

    response = await service_client.post(
        "/api/tasks/load", data=json.dumps(data), headers=headers
    )
    assert response.status == 200

    data = {
        "task_id": 1,
        "language": "python",
        "solution": "print(",
    }

    response = await service_client.post(
        "/api/solutions/submit", data=json.dumps(data), headers=headers
    )
    response_json = response.json()
    assert response_json["verdict"] == "Compilation Error"
    assert response.status == 200


async def test_load_task_and_send_compilation_error_solution(service_client):
    data = {
        "name": "Sum",
        "tests": "1 2#3|3 4#7|5 -1#4",
        "legend": "Sum of two numbers",
        "time_limit": 1000,
        "memory_limit": 1024,
    }
    headers = {"Content-Type": "application/json"}

    response = await service_client.post(
        "/api/tasks/load", data=json.dumps(data), headers=headers
    )
    assert response.status == 200

    data = {
        "task_id": 1,
        "language": "python",
        "solution": "print(",
    }

    response = await service_client.post(
        "/api/solutions/submit", data=json.dumps(data), headers=headers
    )
    response_json = response.json()
    assert response_json["verdict"] == "Compilation Error"
    assert response.status == 200


async def test_load_task_and_send_time_limit_solution(service_client):
    data = {
        "name": "Sum",
        "tests": "1 2#3|3 4#7|5 -1#4",
        "legend": "Sum of two numbers",
        "time_limit": 1000,
        "memory_limit": 1024,
    }
    headers = {"Content-Type": "application/json"}

    response = await service_client.post(
        "/api/tasks/load", data=json.dumps(data), headers=headers
    )
    assert response.status == 200

    data = {
        "task_id": 1,
        "language": "python",
        "solution": "from time import sleep\nsleep(1)",
    }

    response = await service_client.post(
        "/api/solutions/submit", data=json.dumps(data), headers=headers
    )
    response_json = response.json()
    assert response_json["verdict"] == "Time Limit exceeded"
    assert response.status == 200


async def test_load_task_and_send_two_solutions_at_time(service_client):
    data = {
        "name": "Sum",
        "tests": "1 2#3|3 4#7|5 -1#4",
        "legend": "Sum of two numbers",
        "time_limit": 1000,
        "memory_limit": 1024,
    }
    headers = {"Content-Type": "application/json"}

    response = await service_client.post(
        "/api/tasks/load", data=json.dumps(data), headers=headers
    )
    assert response.status == 200

    data_ok = {
        "task_id": 1,
        "language": "python",
        "solution": "from time import sleep\nsleep(0.5)\na, b = map(int, input().split())\nprint(a + b)",
    }

    data_wa = {
        "task_id": 1,
        "language": "python",
        "solution": "from time import sleep\nsleep(0.5)\na, b = map(int, input().split())\nprint(a + b + 1)",
    }

    response_ok = await service_client.post(
        "/api/solutions/submit", data=json.dumps(data_ok), headers=headers
    )
    response_wa = await service_client.post(
        "/api/solutions/submit", data=json.dumps(data_wa), headers=headers
    )
    response_json_ok = response_ok.json()
    assert response_json_ok["verdict"] == "Accepted!"
    assert response_ok.status == 200

    response_json_wa = response_wa.json()
    assert response_json_wa["verdict"] == "Wrong answer!"
    assert response_wa.status == 200
