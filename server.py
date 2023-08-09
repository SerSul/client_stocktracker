from flask import Flask, request, jsonify
import mysql.connector

app = Flask(__name__)

def MySQLconnect():
    try:
        connection = mysql.connector.connect(
            host="",
            user="",
            password="",
            database=""
        )
        cursor = connection.cursor()
        return connection, cursor
    except mysql.connector.Error as err:
        return None, None

@app.route('/products', methods=['GET'])
def get_all_products():
    client_ip = request.remote_addr
    print("IP-адрес клиента:", client_ip)
    connection, cursor = MySQLconnect()
    if connection and cursor:
        query = "SELECT * FROM stocktracker.products"
        cursor.execute(query)
        result = cursor.fetchall()
        cursor.close()
        connection.close()

        columns = [desc[0] for desc in cursor.description]
        products_list = []
        for row in result:
            product = dict(zip(columns, row))
            products_list.append(product)

        response_data = {"status": "success", "data": products_list}
        return jsonify(response_data), 200
    else:
        response_data = {"status": "error", "message": "Ошибка подключения к базе данных!"}
        return jsonify(response_data), 200

@app.route('/products/<int:id>', methods=['GET'])
def get_product_by_id(id):
    connection, cursor = MySQLconnect()
    if connection and cursor:
        query = "SELECT * FROM stocktracker.products WHERE id = %s"
        cursor.execute(query, (id,))
        result = cursor.fetchall()
        cursor.close()
        connection.close()

        columns = [desc[0] for desc in cursor.description]
        products_list = []
        for row in result:
            product = dict(zip(columns, row))
            products_list.append(product)

        if not products_list:
            response_data = {"status": "error", "message": f"Продукт с ID={id} не найден в базе данных!"}
            return jsonify(response_data), 200
        else:
            response_data = {"status": "success", "data": products_list}
            return jsonify(response_data), 200
    else:
        response_data = {"status": "error", "message": "Ошибка подключения к базе данных!"}
        return jsonify(response_data), 200

@app.route('/products/<int:product_id>', methods=['PUT'])
def update_product_by_id(product_id):
    connection, cursor = MySQLconnect()
    if connection and cursor:
        try:
            data = request.get_json()
            if data:
                name = data.get('name')
                price = data.get('price')
                quantity = data.get('quantity')

                check_query = "SELECT * FROM stocktracker.products WHERE id = %s"
                cursor.execute(check_query, (product_id,))
                result = cursor.fetchall()

                if not result:
                    response_data = {"status": "error", "message": f"Продукт с ID={product_id} не найден в базе данных!"}
                    return jsonify(response_data), 200
                else:
                    update_query = "UPDATE stocktracker.products SET "
                    update_values = []

                    if name is not None:
                        update_query += "name = %s, "
                        update_values.append(name)

                    if price is not None:
                        update_query += "price = %s, "
                        update_values.append(price)

                    if quantity is not None:
                        update_query += "quantity = %s, "
                        update_values.append(quantity)

                    # Remove the trailing comma and space
                    update_query = update_query[:-2]

                    update_query += " WHERE id = %s"
                    update_values.append(product_id)

                    cursor.execute(update_query, update_values)
                    connection.commit()
                    response_data = {"status": "success", "message": f"Обновление продукта с ID={product_id} выполнено успешно!"}
                    return jsonify(response_data), 200
            else:
                response_data = {"status": "error", "message": "Ошибка при получении данных из запроса"}
                return jsonify(response_data), 200
        except Exception as e:
            connection.rollback()
            response_data = {"status": "error", "message": f"Ошибка при обновлении продукта в базе данных: {e}"}
            return jsonify(response_data), 200
        finally:
            cursor.close()
            connection.close()
    else:
        response_data = {"status": "error", "message": "Ошибка подключения к базе данных!"}
        return jsonify(response_data), 200

@app.route('/products/<int:id>', methods=['DELETE'])
def delete_product_by_id(id):
    connection, cursor = MySQLconnect()
    if connection and cursor:
        check_query = "SELECT * FROM stocktracker.products WHERE id = %s"
        cursor.execute(check_query, (id,))
        result = cursor.fetchall()

        if not result:
            response_data = {"status": "success", "message": f"Продукт с ID={id} не найден в базе данных!"}
            return jsonify(response_data), 200
        else:
            query = "DELETE FROM stocktracker.products WHERE id = %s"
            cursor.execute(query, (id,))
            connection.commit()
            cursor.close()
            connection.close()
            response_data = {"status": "success", "message": f"Удаление продукта с ID={id} выполнено успешно!"}
            return jsonify(response_data), 200
    else:
        response_data = {"status": "error", "message": "Ошибка при подключении к базе данных!"}
        return jsonify(response_data), 200

@app.route('/products/add', methods=['PUT'])
def create_product():
    connection, cursor = MySQLconnect()
    if connection and cursor:
        data = request.get_json()
        if data:
            id_ = data.get('id')
            name = data.get('name')
            price = data.get('price')
            quantity = data.get('quantity')

            query = "INSERT INTO stocktracker.products (id, name, quantity, price) VALUES (%s, %s, %s, %s)"
            values = (id_, name, quantity, price)
            try:
                cursor.execute(query, values)
                connection.commit()
                response_data = {"status": "success", "message": f"Создание продукта с ID={id_} выполнено успешно!"}
                return jsonify(response_data), 200
            except Exception as e:
                response_data = {"status": "error", "message": "Не удалось создать продукт."}
                return jsonify(response_data), 200
        else:
            response_data = {"status": "error", "message": "Ошибка при получении данных из запроса"}
            return jsonify(response_data), 200
    else:
        response_data = {"status": "error", "message": "Ошибка подключения к базе данных!"}
        return jsonify(response_data), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0')
