package cz.cvut.fit.tjv.realestates.application;

import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.repository.JPACustomerRepository;
import cz.cvut.fit.tjv.realestates.repository.JPAPropertyRepository;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.Mock;

import java.time.LocalDate;
import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class CustomerServiceTest {
    @Mock
    private JPACustomerRepository customerRepository;
    @Mock
    private JPAPropertyRepository propertyRepository;

    @BeforeEach
    public void setUp() {
        customerRepository = mock(JPACustomerRepository.class);
    }

    private static List<Customer> prepareCustomers(List<String> emails) {
        return List.of(
                new Customer(emails.get(0), "Jan", "Novak",
                        LocalDate.of(1995, 9, 11), List.of()),
                new Customer(emails.get(1), "Lukas", "Prochazka",
                        LocalDate.of(2001, 6, 20), List.of()),
                new Customer(emails.get(2), "Test", "Customer",
                        LocalDate.of(1995, 9, 30), List.of())
        );
    }

    @Test
    public void testGetCustomerById() {
        Customer expectedCustomer = new Customer("Java.Java@mail.com", "Java", "Java",
                LocalDate.of(1988, 12, 11), List.of());
        when(customerRepository.findById("Java.Java@mail.com")).thenReturn(Optional.of(expectedCustomer));

        CustomerService customerService = new CustomerServiceImpl(customerRepository, propertyRepository);
        Customer actualCustomer = customerService.getEntityById("Java.Java@mail.com");
        assertNotNull(actualCustomer);
        assertEquals(expectedCustomer, actualCustomer);
    }

    @Test
    public void testGetMultipleCustomersById() {
        List<String> emails = List.of("Jan.Novak@mail.com", "Lukas.Prochazka@mail.com", "Test.Customer@mail.com");
        List<Customer> expectedCustomers = prepareCustomers(emails);
        for (Customer expectedCustomer : expectedCustomers) {
            when(customerRepository.existsById(expectedCustomer.getEmail())).thenReturn(true);
            when(customerRepository.findById(expectedCustomer.getEmail())).thenReturn(Optional.of(expectedCustomer));
        }
        CustomerService customerService = new CustomerServiceImpl(customerRepository, propertyRepository);
        List<Customer> actualCustomers = customerService.getEntitiesByIds(emails);
        assertNotNull(actualCustomers);
        assertEquals(expectedCustomers, actualCustomers);
    }

    @Test
    public void testGetAllCustomers() {
        List<String> emails = List.of("Jan.Novak@mail.com", "Lukas.Prochazka@mail.com", "Test.Customer@mail.com");
        List<Customer> expectedCustomers = prepareCustomers(emails);
        when(customerRepository.findAll()).thenReturn(expectedCustomers);

        CustomerService customerService = new CustomerServiceImpl(customerRepository, propertyRepository);
        List<Customer> actualCustomers = customerService.getAllEntities();
        assertEquals(expectedCustomers.size(), actualCustomers.size());
        for (int i = 0; i < actualCustomers.size(); i++) {
            assertNotNull(actualCustomers.get(i));
            assertEquals(expectedCustomers.get(i), actualCustomers.get(i));
        }
    }

    @Test
    public void testCreateCustomer() {
        Customer testCustomer = new Customer("Java.Java@mail.com", "Java", "Java",
                LocalDate.of(1988, 12, 11), List.of());
        CustomerService customerService = new CustomerServiceImpl(customerRepository, propertyRepository);
        when(customerRepository.save(testCustomer)).thenReturn(testCustomer);
        when(customerRepository.findById("Java.Java@mail.com")).thenReturn(Optional.of(testCustomer));

        Customer savedCustomer = customerService.createEntity(testCustomer);
        assertNotNull(savedCustomer);
        assertEquals(testCustomer, savedCustomer);
        assertNotNull(customerService.getEntityById(testCustomer.getEmail()));
        assertEquals(testCustomer, customerService.getEntityById(testCustomer.getEmail()));
    }

    @Test
    public void testCreateInvalidCustomer() {
        Customer testInvalidCustomer = new Customer("Java@mail.com", "Java", "Java",
                LocalDate.of(1988, 12, 11), List.of());
        CustomerService customerService = new CustomerServiceImpl(customerRepository, propertyRepository);
        when(customerRepository.save(testInvalidCustomer)).thenReturn(testInvalidCustomer);
        when(customerRepository.findById(testInvalidCustomer.getEmail())).thenReturn(Optional.of(testInvalidCustomer));
        assertThrowsExactly(IllegalArgumentException.class, () -> customerService.createEntity(testInvalidCustomer));
    }

    @Test
    public void testUpdateCustomer() {
        Customer oldCustomer = new Customer("Java.Java@mail.com", "Java", "Java",
                LocalDate.of(1986, 12, 11), List.of());
        Customer expectedNewCustomer = new Customer("Java.Java@mail.com", "Java", "Java",
                LocalDate.of(1988, 12, 11), List.of());
        when(customerRepository.existsById("Java.Java@mail.com")).thenReturn(true);
        when(customerRepository.save(any(Customer.class))).thenReturn(expectedNewCustomer);
        when(customerRepository.getReferenceById("Java.Java@mail.com")).thenReturn(oldCustomer);

        CustomerService customerService = new CustomerServiceImpl(customerRepository, propertyRepository);
        Customer updatedCustomer = customerService.updateEntity(expectedNewCustomer);
        assertNotNull(updatedCustomer);
        assertEquals(expectedNewCustomer, updatedCustomer);
    }

}
