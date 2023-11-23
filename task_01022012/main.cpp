/*�������� ���� �ᒺ��� ��������. ����� �ᒺ�� ��������������� ���������� �� �������� ������ �ᒺ���, ������, �
����� �����.
��� �������� ���� ��������. ����� ������� �� ��������� ������������� (����� �� ��������� ����������� ����),
���, �������, ������� �� �������� �����, ������(������ ��� ����� ���������) � ����������� �� �ᒺ��� ��������.
��������� ������ �������� �������� �� ������ �������� �� ������. ��� ����� �������� �� �ᒺ�� �������� ��
���������� �� ������ �������� �� ������, � ����� ���������� �������� �����.
� ������� ������� ������� ������ �ᒺ��� �������� � ���� ����������, ������� �����. ������� �������� ������
��������, �� ��� �������� ������ �������� � ������������� �������. ���������� ������ ���� �������� �����
�����, ��� � ������ � ��� �������� ����� �ᒺ�� ��������, � � ������� ���������� ������� ����� �� �������.*/

#include <iostream>
#include <vector>
#include <string>


class Owner;

class Property {

private:
	std::string     address;
	std::string     name;
	double          price;

	static std::vector<Property*> instances;  // ��������� ��� ���������� ����������
public:
	Property(const std::string& address_, const std::string& name_, double price_)
		: address(address_), name(name_), price(price_) {
		// ��������� ������ �������������� ��'���� � ���������
		instances.push_back(this);
	}

	static Property* createInstance(const std::string& address_, const std::string& name_, double price_) {
		return new Property(address_, name_, price_);  // ������� ��� ��������� ���������� ����� �� ����
	}

	static void destroyInstance(Property* obj);

	// ���������� ��� ��������� ��'����, �� ������ ���������� � ��������� ��� ��������� ��������
	~Property();

	// �������� ������   
	const std::string& getAddress()                  const { return address; }
	const std::string& getName()                     const { return name; }
	double             getPrice()                    const { return price; }
	void               setPrice(const double price_) { price = price_; }
};

// ����������� ���������� �����-����������
std::vector<Property*> Property::instances;


// �������� << ��� ��������� ���������� ��� Property
std::ostream& operator<<(std::ostream& os, const Property& property) {
	os << "���������� " << property.getName() << " (" << property.getAddress() << ") ������� " << property.getPrice();
	return os;
}

class Owner {
private:
	const int               id;
	std::string             firstName;
	std::string             lastName;
	double                  accountBalance;
	std::vector<Property*>  properties;

	static std::vector<Owner*> instances;  // ��������� ��� ���������� ����������
	friend Property::~Property();
public:
	Owner(int id_, const std::string& firstName_, const std::string& lastName_, double accountBalance_)
		: id(id_), firstName(firstName_), lastName(lastName_), accountBalance(accountBalance_) {
		// ��������� ������ �������������� ��'���� � ���������
		instances.push_back(this);
	}

	static Owner* createInstance(int id_, const std::string& firstName_, const std::string& lastName_, double accountBalance_) {
		return new Owner(id_, firstName_, lastName_, accountBalance_);  // ������� ��� ��������� ���������� ����� �� ����
	}

	static void destroyInstance(Owner* obj) {
		// ��������� ��'���� � ���������� �� ���� ���������
		auto it = std::find(instances.begin(), instances.end(), obj);
		if (it != instances.end()) {
			instances.erase(it);
			delete obj;
		}
	}

	// ���������� ��� ��������� ��'����, �� ������ ���������� � ��������� ��� ��������� ��������
	~Owner() {
		auto it = std::find(instances.begin(), instances.end(), this);
		if (it != instances.end()) {
			instances.erase(it);
		}
	}
	// �������� ������
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
		std::cout << "�������� " << firstName << " " << lastName << ":" << std::endl;
		// ������ �������� � ������ properties
		for (const auto& prop : properties) {
			// ��'��� ����, ����� ��������������� sharedPtr
			std::cout << " -- " << *prop << std::endl;
		}
		std::cout << " -- ������� ����� �� ����������� �������: " << accountBalance << "���." << std::endl;
	}

};
// ����������� ���������� �����-����������
std::vector<Owner*> Owner::instances;

void Property::destroyInstance(Property* obj) {


	// ��������� ��'���� � ���������� �� ���� ���������
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
	// ��������� ��������
	Owner owner1(1, "����", "�������", 400000.0);
	Owner owner2(2, "����", "�����", 500000.0);
	{
		// ³���������� ���������� ������� ��������
		std::cout << ">>>>>>>>> ������ �� 01.01.2000 <<<<<<<<<" << std::endl;
		owner1.displayProperties();
		owner2.displayProperties();

		//2 -------------------------------------------------------------------------
		// ��������� ��'���� ��������
		Property property1("123 Main St", "House A", 150000.0);
		Property property2("456 Oak St", "Apartment B", 80000.0);

		// �������� �������� �� �����
		owner1.addProperty(property1);
		owner2.addProperty(&property2);

		// ³���������� ���������� ������� ��������
		std::cout << std::endl << std::endl;
		std::cout << ">>>>>>>>> ������ �� 01.01.2005 <<<<<<<<<" << std::endl;
		owner1.displayProperties();
		owner2.displayProperties();

		//3-------------------------------------------------------------------------
		owner2.addProperty(property1);

		// ³���������� ���������� ������� ��������
		std::cout << std::endl << std::endl;
		std::cout << ">>>>>>>>> ������ �� 01.01.2010 <<<<<<<<<" << std::endl;
		owner1.displayProperties();
		owner2.displayProperties();

		//4-------------------------------------------------------------------------
		{
			Property property3("457 Oak St", "Apartment B", 10000.0);
			owner2.addProperty(property3);

			// ³���������� ���������� ������� ��������
			std::cout << std::endl << std::endl;
			std::cout << ">>>>>>>>> ������ �� 01.01.2015 <<<<<<<<<" << std::endl;
			owner1.displayProperties();
			owner2.displayProperties();
		}


		//5-------------------------------------------------------------------------
		// ³���������� ���������� ������� ��������
		std::cout << std::endl << std::endl;
		std::cout << ">>>>>>>>> ������ �� 01.01.2020 <<<<<<<<<" << std::endl;
		owner1.displayProperties();
		owner2.displayProperties();

	}
	//6-------------------------------------------------------------------------
	// ³���������� ���������� ������� ��������
	std::cout << std::endl << std::endl;
	std::cout << ">>>>>>>>> ������ �� 22.02.2022 <<<<<<<<<" << std::endl;
	owner1.displayProperties();
	owner2.displayProperties();


	return 0;
}
























