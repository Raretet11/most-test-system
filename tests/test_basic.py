import pytest

from testsuite.databases import pgsql


# Start the tests via `make test-debug` or `make test-release`


async def test_load_task(service_client):
    response = await service_client.post(
        '/v1/load?name=Sum%20of%20Two%20Numbers&tests=1%202%\
            233%7C3%204%237%7C5%206%2311',
    )
    assert response.status == 200
    assert response.text == 'OK'


async def test_load_task_and_send_ok_solution(service_client):
    response = await service_client.post(
        '/v1/load?name=Sum%20of%20Two%20Numbers&tests=1\
        %202%233%7C3%204%237%7C5%206%2311',
    )
    assert response.status == 200
    assert response.text == 'OK'
    
    response = await service_client.post(
        '/v1/submit?task-id=1&code=import%20sys%0Ainput_data\
            %20%3D%20sys.argv%5B1%5D%0Aa%2C%20b%20%3D%20map\
                (int%2C%20input_data.split())%0Aprint(a%20%2B%20b)',
    )
    assert response.status == 200
    assert response.text == 'OK'


async def test_load_task_and_send_wa_solution(service_client):
    response = await service_client.post(
        '/v1/load?name=Sum%20of%20Two%20Numbers&tests=1\
            %202%233%7C3%204%237%7C5%206%2311',
    )
    assert response.status == 200
    assert response.text == 'OK'
    
    response = await service_client.post(
        'v1/submit?task-id=1&code=import%20sys%0Ainput_data%20%3D%20sys.\
            argv%5B1%5D%0Aa%2C%20b%20%3D%20map(int%2C%20input_data.split())\
                %0Aprint(a%20%2B%20b+1)',
    )
    assert response.status == 200
    assert response.text == 'WRONG_ANSWER'
