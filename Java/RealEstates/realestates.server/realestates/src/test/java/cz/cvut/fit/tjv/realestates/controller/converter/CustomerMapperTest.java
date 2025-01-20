package cz.cvut.fit.tjv.realestates.controller.converter;

import cz.cvut.fit.tjv.realestates.application.PropertyService;
import cz.cvut.fit.tjv.realestates.controller.dto.CustomerDTO;
import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.domain.Property;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.Mock;

import java.time.LocalDate;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class CustomerMapperTest {
    @Mock
    private PropertyService propertyService;
    private CustomerDTOConverter customerDTOConverter;

    @BeforeEach
    public void setUp() {
        propertyService = mock(PropertyService.class);
    }

    @Test
    void testCustomerToCustomerDTO() {
        Customer customer = new Customer("TestNameTestSurname@mail.com", "TestName1",
                "TestSurname1", LocalDate.of(2000, 1, 4),
                List.of());
        CustomerDTO expectedCustomerDTO = new CustomerDTO("TestNameTestSurname@mail.com",
                "TestName1", "TestSurname1", LocalDate.of(2000, 1, 4),
                List.of(0L, 1L));
        List<Property> properties = List.of(
            new Property(0L, 20, 25_000L,
                    "Brno", "Moravska", 150, customer, List.of()),
            new Property(1L, 12, 30_000L,
                    "Praha", "Sumavska", 12, customer, List.of())
        );
        customer.setSubleases(properties);
        customerDTOConverter = new CustomerDTOConverter(propertyService);
        CustomerDTO actualCustomerDTO = customerDTOConverter.toDTO(customer);
        assertNotNull(actualCustomerDTO);
        assertEquals(expectedCustomerDTO.getEmail(), actualCustomerDTO.getEmail());
        assertEquals(expectedCustomerDTO.getFirstName(), actualCustomerDTO.getFirstName());
        assertEquals(expectedCustomerDTO.getLastName(), actualCustomerDTO.getLastName());
        assertEquals(expectedCustomerDTO.getBirthDate(), actualCustomerDTO.getBirthDate());
        assertEquals(expectedCustomerDTO.getIdSubleases(), actualCustomerDTO.getIdSubleases());
    }

    @Test
    void testCustomerDTOToCustomer() {
        CustomerDTO customerDTO = new CustomerDTO("TestNameTestSurname@mail.com", "TestName2",
                "TestSurname2", LocalDate.of(1999, 11, 4), List.of());
        Customer expectedCustomer = new Customer("TestNameTestSurname@mail.com", "TestName2",
                "TestSurname2", LocalDate.of(1999, 11, 4), List.of());
        customerDTOConverter = new CustomerDTOConverter(propertyService);
        when(propertyService.getEntitiesByIds(customerDTO.getIdSubleases())).thenReturn(expectedCustomer.getSubleases());
        Customer actualCustomer = customerDTOConverter.toEntity(customerDTO);
        assertNotNull(actualCustomer);
        assertEquals(expectedCustomer.getEmail(), actualCustomer.getEmail());
        assertEquals(expectedCustomer.getFirstName(), actualCustomer.getFirstName());
        assertEquals(expectedCustomer.getLastName(), actualCustomer.getLastName());
        assertEquals(expectedCustomer.getBirthDate(), actualCustomer.getBirthDate());
        assertEquals(expectedCustomer.getSubleases(), actualCustomer.getSubleases());
    }
}
