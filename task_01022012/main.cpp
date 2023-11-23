/*Створити клас об’єктів власності. Кожен об’єкт характеризується вказівником на унікальну адресу об’єкта, назвою, а
також ціною.
Далі створити клас власників. Кожен власник має унікальний ідентифікатор (можна за прикладом податкового коду),
ім’я, прізвище, рахунок із залишком коштів, вектор(список або інший контейнер) з вказівниками на об’єкти власності.
Розробити методи передачі власності від одного власника до іншого. При цьому вказівник на об’єкт власності має
переходити від одного власника до іншого, а також відбуватися передача коштів.
В головній функції створіть перелік об’єктів власності з усіма атрибутами, зокрема ціною. Створіть первісний перелік
власників, які вже володіють певною власністю з вищезгаданого переліка. Застосуйте методи зміни власника таким
чином, щоб в одного з них появився новий об’єкт власності, а в другого збільшилася кількість коштів на рахунку.*/

#include <iostream>
#include <vector>
#include <string>


class Owner;

class Property {

private:
	std::string     address;
	std::string     name;
	double          price;

	static std::vector<Property*> instances;  // Контейнер для відстеження екземплярів
public:
	Property(const std::string& address_, const std::string& name_, double price_)
		: address(address_), name(name_), price(price_) {
		// Додавання адреси новоствореного об'єкту в контейнер
		instances.push_back(this);
	}

	static Property* createInstance(const std::string& address_, const std::string& name_, double price_) {
		return new Property(address_, name_, price_);  // Функція для створення екземпляру класу на кучі
	}

	static void destroyInstance(Property* obj);

	// Деструктор для видалення об'єктів, які можуть залишитися в контейнері при завершенні програми
	~Property();

	// Додаткові методи   
	const std::string& getAddress()                  const { return address; }
	const std::string& getName()                     const { return name; }
	double             getPrice()                    const { return price; }
	void               setPrice(const double price_) { price = price_; }
};

// Ініціалізація статичного члена-контейнера
std::vector<Property*> Property::instances;


// Оператор << для виведення інформації про Property
std::ostream& operator<<(std::ostream& os, const Property& property) {
	os << "Нерухомість " << property.getName() << " (" << property.getAddress() << ") вартістю " << property.getPrice();
	return os;
}

class Owner {
private:
	const int               id;
	std::string             firstName;
	std::string             lastName;
	double                  accountBalance;
	std::vector<Property*>  properties;

	static std::vector<Owner*> instances;  // Контейнер для відстеження екземплярів
	friend Property::~Property();
public:
	Owner(int id_, const std::string& firstName_, const std::string& lastName_, double accountBalance_)
		: id(id_), firstName(firstName_), lastName(lastName_), accountBalance(accountBalance_) {
		// Додавання адреси новоствореного об'єкту в контейнер
		instances.push_back(this);
	}

	static Owner* createInstance(int id_, const std::string& firstName_, const std::string& lastName_, double accountBalance_) {
		return new Owner(id_, firstName_, lastName_, accountBalance_);  // Функція для створення екземпляру класу на кучі
	}

	static void destroyInstance(Owner* obj) {
		// Видалення об'єкту з контейнера та його видалення
		auto it = std::find(instances.begin(), instances.end(), obj);
		if (it != instances.end()) {
			instances.erase(it);
			delete obj;
		}
	}

	// Деструктор для видалення об'єктів, які можуть залишитися в контейнері при завершенні програми
	~Owner() {
		auto it = std::find(instances.begin(), instances.end(), this);
		if (it != instances.end()) {
			instances.erase(it);
		}
	}
	// Додаткові методи
	int                 getId()             const { return id; }
	const std::string& getFirstName()      const { return firstName; }
	const std::string& getLastName()       const { return lastName; }
	double              getAccountBalance() const { return accountBalance; }

	void addProperty(Property* property) {
		properties.push_back(property);
		accountBalance -= property->getPrice();
		for (auto& owner : instances) {
			if (owner != this) {
				auto it = std::find(owner->properties.begin(), owner->properties.end(), property);
				if (it != owner->properties.end()) {
					owner->accountBalance += property->getPrice();
					owner->properties.erase(it);
				}
			}
		}
	}
	void addProperty(Property& property) { addProperty(&property); }
	void displayProperties() const {
		std::cout << "Власність " << firstName << " " << lastName << ":" << std::endl;
		// Перебір елементів у векторі properties
		for (const auto& prop : properties) {
			// Об'єкт існує, можна використовувати sharedPtr
			std::cout << " -- " << *prop << std::endl;
		}
		std::cout << " -- залишок коштів на банківському рахунку: " << accountBalance << "грн." << std::endl;
	}

};
// Ініціалізація статичного члена-контейнера
std::vector<Owner*> Owner::instances;

void Property::destroyInstance(Property* obj) {


	// Видалення об'єкту з контейнера та його видалення
	auto it = std::find(instances.begin(), instances.end(), obj);
	if (it != instances.end()) {
		instances.erase(it);
		delete obj;
	}
}

Property::~Property() {

	for (auto& owner : Owner::instances) {
		auto it = std::find(owner->properties.begin(), owner->properties.end(), this);
		if (it != owner->properties.end()) {
			owner->properties.erase(it);
		}
	}

	auto it = std::find(instances.begin(), instances.end(), this);
	if (it != instances.end()) {
		instances.erase(it);
	}
}

int main() {
	system("chcp 1251>nul");

	//1 -------------------------------------------------------------------------
	// Створення власників
	Owner owner1(1, "Іван", "Данелюк", 400000.0);
	Owner owner2(2, "Марія", "Козак", 500000.0);
	{
		// Відображення власностей кожного власника
		std::cout << ">>>>>>>>> Станом на 01.01.2000 <<<<<<<<<" << std::endl;
		owner1.displayProperties();
		owner2.displayProperties();

		//2 -------------------------------------------------------------------------
		// Створення об'єктів власності
		Property property1("123 Main St", "House A", 150000.0);
		Property property2("456 Oak St", "Apartment B", 80000.0);

		// Передача власності за кошти
		owner1.addProperty(property1);
		owner2.addProperty(&property2);

		// Відображення власностей кожного власника
		std::cout << std::endl << std::endl;
		std::cout << ">>>>>>>>> Станом на 01.01.2005 <<<<<<<<<" << std::endl;
		owner1.displayProperties();
		owner2.displayProperties();

		//3-------------------------------------------------------------------------
		owner2.addProperty(property1);

		// Відображення власностей кожного власника
		std::cout << std::endl << std::endl;
		std::cout << ">>>>>>>>> Станом на 01.01.2010 <<<<<<<<<" << std::endl;
		owner1.displayProperties();
		owner2.displayProperties();

		//4-------------------------------------------------------------------------
		{
			Property property3("457 Oak St", "Apartment B", 10000.0);
			owner2.addProperty(property3);

			// Відображення власностей кожного власника
			std::cout << std::endl << std::endl;
			std::cout << ">>>>>>>>> Станом на 01.01.2015 <<<<<<<<<" << std::endl;
			owner1.displayProperties();
			owner2.displayProperties();
		}


		//5-------------------------------------------------------------------------
		// Відображення власностей кожного власника
		std::cout << std::endl << std::endl;
		std::cout << ">>>>>>>>> Станом на 01.01.2020 <<<<<<<<<" << std::endl;
		owner1.displayProperties();
		owner2.displayProperties();

	}
	//6-------------------------------------------------------------------------
	// Відображення власностей кожного власника
	std::cout << std::endl << std::endl;
	std::cout << ">>>>>>>>> Станом на 22.02.2022 <<<<<<<<<" << std::endl;
	owner1.displayProperties();
	owner2.displayProperties();


	return 0;
}
























